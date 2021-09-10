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

int i, tmp, j, k, t;
int s_socket;
int entity_length;

struct sockaddr_in addr;

char * statusline;
char * entity;

unsigned char targetip[4] = { 142,250,186,78 };
char request[5000],response[10000];

// Variabile che indica se è presente il transfer-encoding di tipo chunked
int isChunked;
// Variabile che indica se è presente la content-length
int isCl;
// Counter
int jj;


int main() {

  s_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (s_socket == -1){

    tmp = errno;
    perror("Socket fallita");
    printf("i=%d errno=%d\n",i,tmp);
    return 1;

  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(80);
  addr.sin_addr.s_addr = *(unsigned int*)targetip;

  if (connect(s_socket,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {

    perror("Connect fallita");

  }

  //printf("%d\n", s_socket);
  for (int index = 0; index < 10; index++) {

      sprintf(request, "GET / HTTP/1.1\r\nConnenction: keep-alive\r\nHost:www.google.com\r\n\r\n");

      if (write(s_socket, request, strlen(request)) == -1) {

          perror("write fallita");
          return 1;
      }

      bzero(h, sizeof(struct header)*100);
      statusline = h[0].n = response;

      for (j = 0, k = 0; read(s_socket, response+j, 1); j++){

          if (response[j] == ':' && (h[k].v==0)){

              response[j]=0;
              h[k].v=response+j+2;
          }
          else if((response[j]=='\n') && (response[j-1]=='\r') ){

              response[j-1]=0;
              if(h[k].n[0]==0) break;
              h[++k].n=response+j+1;
          }
      }

      entity_length = -1;

      printf("Status line = %s\n",statusline);

      isChunked = 0;
      isCl = 0;

      for(i = 1; i < k; i++) {

          if(strcmp(h[i].n, "Content-Length") == 0) {

              entity_length = atoi(h[i].v);
              //printf("* (%d) ",entity_length);
              // Indico se ho trovato la content-length
              isCl = 1;
          }
          else if (strcmp(h[i].n, "Transfer-Encoding") == 0) {

              if (strcmp(h[i].v, "chunked") == 0) {

                  printf("T-C %s\n", h[i].v);
                  // Indico se ho trovato la transfer-Encoding
                  isChunked = 1;
              }
          }

          printf("%s ----> %s\n",h[i].n, h[i].v);
      }

      if(entity_length == -1) entity_length = 1000000;
      entity = (char * ) malloc(entity_length);
      // Se il transfer-encoding è di tipo chanked
      if (isChunked){

          // Pongo la entity-length a zero
          entity_length = 0;
          // dichiaro ed inizializzo i seguenti valori
          // Lunghezza del chunk (chunk_length) fine ringa (end_pointer)
          int chunk_length = 0, end_pointer = 0;
          // Puntatore alla memoria di tipo malloc
          char *chunk_size = malloc(100);
          do {
              // leggo dal socket il valore e lo scrivo nel chunk_size alla posizione
              // chunksize+jj
              for(jj=0; read(s_socket, (chunk_size + jj), 1) > 0; ++jj){

                // Se jj > 0 e il caratteri in posizione jj e jj-1 sono rispettivamente LF e CR
                // (CR='\r', LF='\n')
                if ((jj > 0) && (chunk_size[jj] == '\n') && (chunk_size[jj-1] == '\r')){

                    // Pongo il terminatore 0 oppure '\0' alla posizione del CR
                    // Per indicare dove termina
                    chunk_size[jj-1] = 0;
                    // Esco dal ciclo
                    break;
                }
              }
              // Rimetto a 0 il valore
              chunk_length = 0;
              if (strlen(chunk_size) > 0) {

                  // "string to long " long int strtol(const char *str, char **endptr, int base)
                  // prende come parametri: il puntatore all'inizio della stringa, un puntatore
                  // dichiarato ma inizializzato (non indispensabile) e il sistema numerico
                  // (ottale, decimale, esadecimale ecc...)
                  // Mi ritorna in long il valore della stringa (se presenti anche dei caratteri
                  // grazie al secondo puntaore si ferma prima e ritorna la loro posizione ad esso)
                  // In questo caso converto e il valore esadecimale di chunk_size in long
                  // e successivamente faccio il cast ad int
                  // Quello che ho di ritorno è la lunghezza del chunked body
                  chunk_length = (int)strtol(chunk_size, NULL, 16);
              }
              // Salvo la lunghezza di tutti i chunk ricevuti fino ad ora per avere poi il valore
              // dell'intera entity-length
              entity_length = entity_length + chunk_length;
              // Rialloco la memoria precedentemente allocata a entity
              // Fornisco il puntatore all'area di memoria che voglio ridimensionare (entity)
              // e la dimensione della nuova area di memoria (entity_length)
              // Essendo di tipo void devo fare il cast
              entity = (char*) realloc(entity,entity_length);

              // Leggo dal socket i byte e li posiziono in  entity + ep + jj fino a chunk_length-jj
              // Incremento jj dei bite letti (t)
              for(jj=0; (t = read(s_socket, entity + end_pointer + jj, chunk_length - jj ) ); jj+=t);
              // Aggiorno ep del valore jj
              end_pointer += jj;
              //
              read(s_socket, chunk_size, 1);
              //
              read(s_socket, chunk_size+1 ,1);

              // se i valori iniziali di chunk_size non corrispondono ai CRLF
              if( ( chunk_size[0] != '\r' ) || ( chunk_size[1] != '\n' ) ){

                  // Stampa un errore nel parsing
                  printf("Errore di parsing. Mancano CR e LF.\n");
              }

          } while (chunk_length > 0);
          // Libera l'area di memoria dedicata a chunk_sie
          free(chunk_size);
      }
      // Se non è presente il transfer-encoding di tipo chunked utilizza la content-lenght
      else if (isCl){

          for(j = 0; (t = read(s_socket, entity + j, entity_length - j)) > 0; j += t);
          printf("j= %d\n", j);
      }

      for(i = 0; i < entity_length; i++) {

          printf("%c", entity[i]);
          int a = 1;
      }
      // Libera l'area di memoria dedicata all'entity
      free(entity);
  }
}
