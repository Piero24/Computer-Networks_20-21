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

unsigned char targetip[4] = {88,80,187,84};
char request[5000],response[10000];

// Puntatore alla sottostringa contenente la dimensione del file
char *totLenght;
// Puntatore al file
FILE *fpoint;

int main() {

  // Apro il file immagine in cui scrivere
  fpoint = fopen("image.jpg", "a");

  // Il totale dei byte di cui è composta l'immagine sono 1674791

  // quanti byte prendere ad ogni iterazione del ciclo
  int dim = 10000;
  // Totale dei byte dell'immagine all'inizio quando non si cognose la dimensione
  // Si sa soltanto che bisogna prendere 10'000 byte alla volta
  // (Questo valore verrà incrementa alla prima iterazione del ciclo
  // quando si saprà la vera dimensione del file)
  long total = 10000;

// se i byte presi (arr) sono minori della dimensione del file (total)
// incrementa arr di 10'000 byte
for (int arr = 0; arr < total; arr += dim) {

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
  // richiesta inviata al serve con al suo interno il range di byte che voglio prelevare
  sprintf(request, "GET /image.jpg HTTP/1.1\r\nRange: bytes=%d-%d\r\nHost: 88.80.187.84\r\n\r\n", arr, arr + dim - 1);

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

  for(i = 1; i < k; i++) {

    if(strcmp(h[i].n, "Content-Length") == 0) {

      entity_length = atoi(h[i].v);
      printf("* (%d) ",entity_length);

    }
    // Cerco l'header content range che mi ha inviato il server
    else if(strcmp(h[i].n, "Content-Range") == 0){

      // per ogni carattere al suo interno
      for (int asa = 0; asa < strlen(h[i].v); asa++) {
        // Trovo dove inizia la sottostringa contenente il numero totale dei byte
        // di cui è composto il file
        if (h[i].v[asa] == '/') {

          // Assegno questo numero al puntatore
          totLenght = h[i].v + asa + 1;
        }
      }
    }
    printf("%s ----> %s\n",h[i].n, h[i].v);
  }
  // Faccio un cast ad int del numero totale di byte
  total = atoi(totLenght);
  //printf("%d\n", atoi(totLenght));


  if(entity_length == -1) entity_length = 1000000;

  entity = (char * ) malloc(entity_length);

  for(j = 0; (t = read(s_socket, entity + j, entity_length - j)) > 0; j += t);

  printf("j= %d\n", j);

  for(i = 0; i < j; i++) {

    // Stampo nel file carattere per carattere il range che mi ha inviato il server
    fprintf(fpoint, "%c", entity[i]);
    //printf("%c", entity[i]);
  }
  // Chiudo il socket
  close(s_socket);
}
// Chiudo il file
fclose(fpoint);

}
