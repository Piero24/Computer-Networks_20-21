#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

// Strutura dati dell'header
struct header{
char * n;
char * v;
}h[100];

int i, tmp, j, k, t;
int s_socket;

//Inizializzo la var che indica la lunghezza della Content-Length
int entity_length;

struct sockaddr_in addr;

// Puntatore alla statusline
char * statusline;

char * entity;

// <indirizzo ip del server 216.58.213.100 >
// Array contenete l'indirzzo ip del server
//unsigned char targetip[4] = {143,204,182,107};
//unsigned char targetip[4] = { 172,217,169,3};
//unsigned char targetip[4] = { 216, 58 ,213,100 };
//unsigned char targetip[4] = { 213,92,16,101 };
unsigned char targetip[4] = { 172,217,21,68 };


// sono i 2 buffer per la richiesta e la risposta
char request[5000],response[10000];

int main() {

  // chiamata a sistema che apre una comunicazione e restituisce un INT,
  // che è un File Descriptor ovvero l’indice della tabella con tutto ciò
  // che serve per gestire la comunicazione
  // int socket(int domain, int type, int protocol);
  s_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (s_socket == -1){

    // Liberia degli errori
    tmp = errno;

    // Printa testo di errore
  	perror("Socket fallita");

    // Printa  i dati relativi all'errore
  	printf("i=%d errno=%d\n",i,tmp);
  	return 1;

  }

// Indirizzo ipv4 del client che fa la richiesta
addr.sin_family = AF_INET;
// Port del client
// host to network short (htons)
addr.sin_port = htons(80);
// Puntatore all'array contenente l'indirizzo ip del server
addr.sin_addr.s_addr = *(unsigned int*)targetip;

// connect == -1 da errore

// connect(): è una chiamata a sistema per connettersi al socket riferito dal
// file descriptor sockfd all’indirizzo specificato da addr. addrlen specifica
// la lunghezza dell’indirizzo.

// sockaddr è generica

// sockaddr_in è dedicata all' IPv4 cioè per essere usata con
// l'Internet Protocol (TCP o UDP per il trasporto).
if (connect(s_socket,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {

  // Printa testo di errore
	perror("Connect fallita");

}
printf("%d\n", s_socket);

sprintf(request, "GET / HTTP/1.0\r\nHost:www.google.com\r\n\r\n");

// la write riceve un file descriptor fd (in questo caso corrisponde a s)
// Un puntatore ad un array di caratteri (request)
// la dimensione del buffer (o array di carattery) (strlen(request)))
// se -1 da errore
if (write(s_socket, request, strlen(request)) == -1) {

  perror("write fallita");
  return 1;
}

// Metto a zero tutti bite nell'header
bzero(h, sizeof(struct header)*100);

statusline = h[0].n = response;

for (j = 0, k = 0; read(s_socket, response+j, 1); j++){

  if (response[j] == ':' && (h[k].v==0)){

    // é il carattere che indica la terminazione della stringa
    response[j]=0;

    h[k].v=response+j+2;
  }

  else if((response[j]=='\n') && (response[j-1]=='\r') ){

    // é il carattere che indica la terminazione della stringa
    response[j-1]=0;

    if(h[k].n[0]==0) break;

    h[++k].n=response+j+1;
  }
}

// inizializzo la lunghezza dell'entity body
entity_length = -1;

printf("Status line = %s\n",statusline);

for(i = 1; i < k; i++) {

  // verifico se la Content-Length è presente o meno nello header
  if(strcmp(h[i].n, "Content-Length") == 0) {

    // se la Content-Length è presente assegno il valore della lunghezza della Content-Length
    // atoi() converte ina stringa (o un puntatore ad una stringa) in intero
    entity_length = atoi(h[i].v);

    // Stampo la lunghezza della Content-Length che corrisponde alla entity_length
    printf("* (%d) ",entity_length);

  }

  printf("%s ----> %s\n",h[i].n, h[i].v);

}

// se la Content-Length NON è presente assegno il valore 1000000 alla entity_length
if(entity_length == -1) entity_length = 1000000;

entity = (char * ) malloc(entity_length);

for(j = 0; (t = read(s_socket, entity + j, entity_length - j)) > 0; j += t);
//if ( t == -1) { perror("Read fallita"); return 1;}
printf("j= %d\n", j);

for(i = 0; i < j; i++) printf("%c", entity[i]);

}
