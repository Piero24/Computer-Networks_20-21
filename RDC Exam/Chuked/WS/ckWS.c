#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

#define CHUNKED_SIZE 20

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

  int a;

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

     } else {

       // Preparo il buffer di dimenione CHUNKED_SIZE
       char buffer[CHUNKED_SIZE];

       // Indico al client che i dati verranno inviati chuked
       sprintf(response,"HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
       write(s2, response, strlen(response));
       do {
               // Inizializzo la variabile a al valore 0
               a = 0;
               // Se a < della dimensione del buffer e il carattere preso dal
               // file non è EOF
               while((a < CHUNKED_SIZE) && (c = fgetc(fin)) != EOF)
                       // Metti il carattere nel buffer incrementato di 1
                       buffer[a++] = c;
               // Scrivi nella response il valore di a in esadecimale
               sprintf(response,"%X\r\n",a);
               // Invialo al client
               write(s2, response, strlen(response));
               // Invia il buffer di dimensione a al client
               write(s2, buffer, a);
               // Scrivi nella response i caratteri \r\n
               sprintf(response,"\r\n");
               // Invialo al client
               write(s2, response, strlen(response));

       }
       // feof() Ritorna 0 se non è stata raggiunta la fine del file
       while(!feof(fin));
               // Chiudi il file
               fclose(fin);
       // Scrivi nella response il valore di terminazione
       sprintf(response,"%X\r\n",0);
       // Invialo al client
       write(s2, response, strlen(response));
       // Scrivi nella response i caratteri \r\n
       sprintf(response,"\r\n");
       // Invialo al client
       write(s2, response, strlen(response));
     }

     close(s2);
 }
}
