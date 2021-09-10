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

   printf("Command line = %s\n", h[0].n);

   for(i = 1; i < k; i++) {

     printf("%s ----> %s\n", h[i].n, h[i].v);

   }
   // Indirizzo ip del client
   printf("IP address is: %s\n", inet_ntoa(remote_addr.sin_addr));
   // Port del client
   printf("port is: %d\n", (int)ntohs(remote_addr.sin_port));

   // Inizializzo la dimensione del buffer
   size_t bufsize = 1000;
   // Inizializzo il buffer di tipo char
   char buffer[bufsize];
   // Indica se ho tentato o meno di visitare la risorsa la prima volta
   int haveVisit = 0;

   // Apro il file e ne leggo ogni riga
   FILE *fileP = fopen("iplist.txt", "r");
   while (fgets(buffer, bufsize, fileP)) {

       // Sostituisco l'ultimo carattere della riga '\n' con il carattere di
       // fine stringa
       buffer[strlen(buffer)-1] = '\0';
       //printf("%s\n", buffer);
       // Controllo se gli ip nel file sono uguali a quello del visitatore
       if (strcmp(buffer, inet_ntoa(remote_addr.sin_addr)) == 0 ){

           // Se ho un riscontro imposto il valore di verifica a 1
           haveVisit = 1;
           break;
       }
   }
   // Chiudo il file
   fclose(fileP);



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
     FILE *fint = fopen("notfound.html", "rt");
     while ( (c = fgetc(fint)) != EOF ) {

         write(s2, &c, 1);
     }
     fclose(fint);
     // FINO A QUI

     } else {

         // Se non ho già tentato di accedere alla risorsa
         if(!haveVisit){

             // Creo un puntatore all'indirizzo ip del client
             char *newBuffer = inet_ntoa(remote_addr.sin_addr);
             // Apro il file con gli indirizzi ip
             FILE *fileP = fopen("iplist.txt", "a");
             // Inserisco nel file l'ip del client
             fprintf(fileP,"%s\n",newBuffer);
             // Chiudo il file
             fclose(fileP);

             // Invio al client la responce
             sprintf(response,"HTTP/1.1 307 Temporary Redirect\r\nRetry-After: 10\r\n\r\n");
             write(s2,response,strlen(response));

             // Scrive nel socket del client il file che dice che non può
             // accedere alla risorsa
             FILE *fnot = fopen("disp1.html", "rt");
             while ( (c = fgetc(fnot)) != EOF ) {

                 write(s2, &c, 1);
             }
             fclose(fnot);

         } else {
             // Se ho già avuto accesso al file

             // Inizializzo un nuovo buffer
             size_t newBuf_size = 1000;
             char newBuf[newBuf_size];
             // Apro il file in lettura
             FILE *fileP = fopen("iplist.txt", "r");
             // Apro un nuovo file
             FILE *fileP2 = fopen("newiplist.txt", "a");

             // Per ogni riga del vecchio file
             while (fgets(newBuf, newBuf_size, fileP)) {

                 //printf("L'IP del file è: %s",newBuf);
                 //printf("Il mio IP è: %s\n",inet_ntoa(remote_addr.sin_addr));
                 int tt;
                 // Sostituisco l'ultimo carattere della riga '\n' con il carattere di
                 // fine stringa
                 newBuf[strlen(newBuf)-1] = '\0';
                 // Se l'ip del client e quello nel file non sono uguali
                 if ((tt = strcmp(newBuf, inet_ntoa(remote_addr.sin_addr)) != 0 )){

                     //printf("%d\n",tt);
                     // Scrivi l'ip nel nuovo file
                     fprintf(fileP2, "%s\n", newBuf);
                 }
             }
             // Chiudi entrambi i file
             fclose(fileP);
             fclose(fileP2);

             // Rimuovi il vecchio file
             remove("iplist.txt");
             // Rinomina il nuovo file come quello vecchio
             rename("newiplist.txt", "iplist.txt");

             sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");
             write(s2,response,strlen(response));

             while ( (c = fgetc(fin)) != EOF ) {

                 write(s2, &c, 1);
             }
             fclose(fin);
         }
     }
     close(s2);
 }
}
