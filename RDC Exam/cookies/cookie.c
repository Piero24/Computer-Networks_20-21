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

  // Dichiaro l'indice del cookie tra gli header
  int cookieIndex;
  // per leggere il file
  int cNotFound;
  // puntatori ai file
  FILE *notF;
  FILE *fin2;

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

   cookieIndex = -1;

   for(i = 1; i < k; i++) {

     printf("%s ----> %s\n", h[i].n, h[i].v);

     // Prendo l'indice i degli header che contengono i cookie
     if (strncmp("Cookie", h[i].n, 100) == 0) {

       // Lo assegno a cookieIndex
       //printf("Il Cookie è all'indice: %d\n", i);
       cookieIndex = i;

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

      // EXTRA NON RICHIESTO
      // Apre il file notfound.html se il file richiesto in path non esiste
      notF = fopen("./notfound.html", "rt");

      // stampa il contenuto del file notfound.html
      while ((cNotFound = fgetc(notF)) != EOF ) {

        write(s2, &cNotFound, 1);
      }

      fclose(notF);
      // *** FINE EXTRA ***
      fclose(fin);

    } else {

      // Se il file richiesto dal cliet in path è disp2.html
      if (strncmp("disp2.html", path + 1 , strlen(path)) == 0) {

        // Verifico tramite il cookie che abbia visitato il file disp1.html
        if (strncmp("SID=31d4d96e407aad42", h[cookieIndex].v, 100) == 0) {

          // Invio al cliente la responce con le info
          sprintf(response,"HTTP/1.1 200 OK\r\n");
          write(s2,response,strlen(response));
          // Setto il nuovo cookie per cancellare il precedente
          // Con questo nuovo cookie il cliente non potrà accedere nuovamente
          // alla pagina disp2.html se prima non ha rivisitato la pagina disp1.html
          // NOTA: I Cookie sono casuali, si può mettere quello che si vuole
          // Basta mantenere la sintassi Set-Cookie: ...\r\n (doppio \r\n solo
          // alla fine di tutti gli header)
          sprintf(response, "Set-Cookie: SID=31d4d96e407aad4233\r\n\r\n");
          write(s2, response, strlen(response));

          // Stampo la pagina disp2.html
          while ( (c = fgetc(fin)) != EOF ) {

            write(s2, &c, 1);

          }

          fclose(fin);

        } else {
          // Essendo che non è stata visitata la pagina disp1.html

          // Se la pagina notdisp2.html è presente
          if ((fin2 = fopen("notdisp2.html", "rt")) != NULL){

            // Indico al client il reindirizzamento ad una nuova pagina
            sprintf(response,"HTTP/1.1 307 Temporary Redirect\r\n\r\n");
            write(s2,response,strlen(response));

            // Stampo la pagina notdisp2.html
            while ( (c = fgetc(fin2)) != EOF ) {

              write(s2, &c, 1);

            }

            fclose(fin2);

          }
        }
      } else {

        // Se visita altre pagine  si manda al client il cookie per visitare
        // la pagina disp2.html

        // NOTA:
        // Se si vuole settare che il cookie venga dato solo dalla pagina disp1.html
        // basta decommentare l'if sottostante e spostare all'interno delle grafe
        // Fino a dove è indicato
        // if (strncmp("disp1.html", path + 1 , strlen(path)) == 0) {}
        sprintf(response,"HTTP/1.1 200 OK\r\n");
        write(s2,response,strlen(response));
        sprintf(response, "Set-Cookie: SID=31d4d96e407aad42\r\n\r\n");
        write(s2, response, strlen(response));

        while ( (c = fgetc(fin)) != EOF ) {

          write(s2, &c, 1);

        }

        fclose(fin);

        // *** FINO A QUESTO COMMENTO E NON OLTRE ***
      }
    }

    close(s2);

 }
}
