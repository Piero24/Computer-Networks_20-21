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

// Alfabeto
unsigned char alphabet[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "abcdefghijklmnopqrstuvwxyz"
                             "0123456789+/";

// Metodo per la codifica in base64 delle password
int base64enc(char* in, int len, char* to){
    int i, j;
    char * o,* s;

    for(i=0, j=0; i<len; i+=3, j+=4)
    {

        o = in + i;
        s = to + j;

        s[0] = alphabet[o[0] >> 2];

        //printf("%d ", len-i);
        if (len-i == 1)
        {
            s[1] = alphabet[(o[0] & 0x3) << 4];
            s[2] = s[3] = '=';
        }
        else if (len -i == 2)
        {
            s[1] = alphabet[((o[0] & 0x3) << 4) | (o[1] >> 4)];
            s[2] = alphabet[(o[1] & 0xF) << 2];
            s[3] = '=';
        }
        else{
            s[1] = alphabet[((o[0] & 0x3) << 4) | (o[1] >> 4)];
            s[2] = alphabet[((o[1] & 0xF) << 2) | (o[2] >> 6)];
            s[3] = alphabet[o[2] & 0x3F];
        }
    }
    return j+1;
}



int main() {

  // per leggere il file
  int cNotFound;
  // puntatori ai file
  FILE *notF;
  // puntatole a username e password in chiaro
  char * userPass;
  // indica le username e pass corrispondono
  int count;
  // indice dell'header in cui trovare la password codificata
  int authorIndex;

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
  addr.sin_port = htons(12121);
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

   authorIndex = -1;

   for(i = 1; i < k; i++) {

     printf("%s ----> %s\n", h[i].n, h[i].v);

     // Trova tra gli header dove è posizionato quello contenente come valore
     // il codice in base64 che codifica username e password
     if (strncmp("Authorization", h[i].n, 100) == 0){

       // Salva l'indice nel quale trovarlo
       authorIndex = i;

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
     notF = fopen("./notfound.html", "rt");

     while ((cNotFound = fgetc(notF)) != EOF) {

       write(s2, &cNotFound, 1);

     }

     fclose(notF);
     // FINO A QUI NON E' NECESSARIO

     } else {

       // Pongo a zero il count che indica se username e pass sono corretti
       count = 0;

       // Se ha inviato un codice di autenticazione
       if (authorIndex != -1) {

         // password e username in chiaro nel formato username:password
         userPass = "nome:matricola";
         // dove salvare la codifica in base64 dell'username e pass
         char encoded[100];
         //printf("%s\n", h[authorIndex].v+6);
         // Codifico in base64 username e pass e li posiziono in encoded
         base64enc(userPass, strlen(userPass), encoded);
         //printf("%s\n", encoded);
         // Comparo il codice fornito dal client e quello appena codificato
         count = !strcmp(h[authorIndex].v+6, encoded);
         //printf("%d\n", count);
        }
        // Se corrispondono
       if(count){

         sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");
         write(s2,response,strlen(response));

         while ((c = fgetc(fin)) != EOF) {

           write(s2, &c, 1);

         }


       }else{
         // Se non corrispondono

         // informa l'utente che non è autorizzato
         sprintf(response,"HTTP/1.1 401 Unauthorized\r\n");
         write(s2,response,strlen(response));
         // Invita l'utente a riprovare nuovamente con le credenziali
         sprintf(response,"WWW-Authenticate: Basic realm=\"LOGIN\"\r\n\r\n");
         write(s2,response,strlen(response));

       }

       fclose(fin);
     }

     close(s2);
 }
}
