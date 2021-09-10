#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>


struct header{
char * n;
char * v;
}h[100];

int tmp;

int main() {

  struct sockaddr_in addr,remote_addr;

  int i,j,k,s,t,s2,c;
  socklen_t len;
  char command[100];
  FILE * fin;
  int yes = 1;

  char * commandline;
  char * method, *path, *ver;
  char request[5000],response[10000];

  // Dichiaro l'indice della referer
  int refererIndex;
  // Dichiaro il puntatore al file se il file non esiste
  FILE *notF;
  int art;
    // Dichiaro il puntatore al file dei referer in blacklist
  FILE *blackL;
  // Dichiaro il buffer di confronto e la sua dimensione
  size_t bufsize = 1000;
  char buffer[bufsize];


  s =  socket(AF_INET, SOCK_STREAM, 0);

  if ( s == -1 ){

    perror("Socket fallita");
    return 1;

  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12101);
  addr.sin_addr.s_addr = 0;

  t = setsockopt(s, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int));

  if (t == -1) {

    perror("setsockopt fallita");
    return 1;

  }

  if (bind(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {

    perror("bind fallita");
    return 1;
  }

  if (listen(s, 225) == -1) {

    perror("Listen Fallita");
    return 1;
  }

  len = sizeof(struct sockaddr_in);

  while(1) {

    s2 =  accept(s, (struct sockaddr *)&remote_addr, &len);

   if (s2 == -1) {

     perror("Accept Fallita");
     return 1;

   }

   bzero(h, 100 * sizeof(struct header *));

   commandline = h[0].n = request;

   for( j = 0 ,k = 0; read(s2, request+j, 1); j++) {

       if(request[j] == ':' && (h[k].v == 0)) {

         request[j] = 0;
         h[k].v = request + j + 2;

       }
       else if((request[j] == '\n') && (request[j - 1] == '\r')) {

         request[j - 1] = 0;
         if(h[k].n[0] == 0) break;
         h[++k].n = request + j + 1;

       }
   }

   // Inizializzo l'indice di referer indicato che non è presente
   refererIndex = -1;

   printf("Command line = %s\n", h[0].n);

   for(i = 1; i < k; i++) {

     printf("%s ----> %s\n", h[i].n, h[i].v);

     // Cerco tra gli header il Referer
     if (strcmp("Referer", h[i].n) == 0) {

       //printf("Il referer è: %s\n", h[i].v);
       // Se lo trovo imposto la variabile con il valore relavio alla sua
       // posizione nella struttura header
       refererIndex = i;

     }

   }

   method = commandline;

   for(i = 0; commandline[i] != ' '; i++){} commandline[i] = 0; path = commandline + i + 1;
   for(i++; commandline[i] != ' '; i++){} commandline[i] = 0; ver = commandline + i + 1;

   printf("method=%s path=%s ver=%s\n", method, path, ver);

   if(strncmp(path, "/cgi-bin/", 9) == 0) {

     sprintf(command, "%s > tmpfile.txt", path + 9);
     printf("Eseguo il comando %s\n", command);

     t = system(command);

     if (t != -1)

       strcpy(path + 1, "tmpfile.txt");
     }


     if ((fin = fopen(path + 1, "rt")) == NULL){

       sprintf(response, "HTTP/1.1 404 Not Found\r\n\r\n");
       write(s2, response, strlen(response));

       // NON NECESSARIO FINO A DOVE INDICATO
       // Ritorna la pagina 404 not found se l'elemento non è disponibile
       notF = fopen("notfound.html", "r");
       while ( (art = fgetc(notF)) != EOF ) {

           write(s2, &art, 1);
       }
       // FINO A QUI NON E' NECESSARIO
       fclose(notF);

       } else {

         // Dichiaro e inizializzo la variabile che mi indica se il refeer è
         // Tra quelli in blacklist
         int isInList = -1;

         // Apro il file dei refer in blacklist
         if ((blackL = fopen("blackList.txt", "r")) == NULL) {

           printf("Error! File cannot be opened.");
           // Stampa errore se il file non è presente
           exit(1);
         }

         // Se la riga del file esiste ed è presente una refere negli header
         while((fgets(buffer, bufsize, blackL) != 0) && (refererIndex != -1)) {

           //printf("BUFFER LEN %lu\n", strlen(buffer));
           // Tolgo il carattere \n dall'ultima posizione della riga presa
           // da file
           buffer[strlen(buffer)-1] = '\0';

           // Faccio la comparazione tra la riga del file senza il \n e il
           // referer presente nel header
           if (strcmp(h[refererIndex].v + 7, buffer) == 0) {

             //printf("%s\n", buffer);
             // Se uguali indico che il referer è in blackList
             isInList = 1;

           }
         }

         // Se il referer non è presente oppure non è in black list
         if(isInList != 1) {

           sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");
           write(s2,response,strlen(response));

           while ( (c = fgetc(fin)) != EOF ) {

             write(s2, &c, 1);

           }

           fclose(fin);

         } else {
           // Se il referer è in blacklist

           //printf("%s\n", h[refererIndex].v + 7);
           // Indica il redirect
           sprintf(response,"HTTP/1.1 307 Temporary Redirect\r\n");
           write(s2,response,strlen(response));
           // Indica dove verrà fatto il redirect
           // Ossia alla pagina da dove si è arrivati
           sprintf(response,"Location: %s\r\n\r\n", h[refererIndex].v + 7);
           write(s2,response,strlen(response));

         }
       }

       close(s2);
   }
  }
