#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;

// Strutura dati dell'header
struct header{
char * n;
char * v;
}h[100];

int main() {
// Puntatore alla statusline
char * statusline;

struct sockaddr_in addr;

int entity_length;
int i,j,k,s,t;
// sono i 2 buffer per la richiesta e la risposta
char request[5000],response[10000];
char * entity;
// Array contenete l'indirzzo ip del server
unsigned char targetip[4] = { 216, 58 ,213,100 };
//unsigned char targetip[4] = { 213,92,16,101 };


// Socket(): è una chiamata a sistema che crea un endpoint (apre una comunicazione)
// e restituisce un INT, che è un File Descriptor ovvero l’indice della tabella
// con tutto ciò che serve per gestire la comunicazione.

// Domain:AF_INET:ipv4

// SOCK_STREAM: apre una comunicazione e invia una sequenza di byte illimitata, che
// si ferma solo quando viene chiusa la comunicazione. Usata per l’invio dei file.

// Protocol:disolito0,htons(ETH_P_ALL)perethernet.
s =  socket(AF_INET, SOCK_STREAM, 0);

// -1 se c'è un errore
if ( s == -1 ){

	tmp=errno;
	// Printa testo di errore
	perror("Socket fallita");
	// Printa  i dati relativi all'errore
	printf("i=%d errno=%d\n",i,tmp);
	return 1;
	}

// Indirizzo ipv4 del client che fa la richiesta
addr.sin_family = AF_INET;
// Port del client
addr.sin_port = htons(80);
// Puntatore all'array contenente l'indirizzo ip del server
addr.sin_addr.s_addr = *(unsigned int*)targetip; // <indirizzo ip del server 216.58.213.100 >

// connect == -1 da errore

// connect(): è una chiamata a sistema per connettersi al socket riferito dal
// file descriptor sockfd all’indirizzo specificato da addr. addrlen specifica
// la lunghezza dell’indirizzo.

// sockaddr è generica

// sockaddr_in è dedicata all' IPv4 cioè per essere usata con
// l'Internet Protocol (TCP o UDP per il trasporto).
if ( -1 == connect(s,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)))

	// Printa testo di errore
	perror("Connect fallita");

printf("%d\n",s);

// Come una printf ma al posto di scrivere a schermo scrive su una stringa (in questo caso sulla request)
for(int iter=0;iter<1;iter++){

	// Come una printf ma al posto di scrivere a schermo scrive su una stringa (in questo caso sulla request)
	sprintf(request,"GET /pluto HTTP/1.0\r\nConnection:keep-alive\r\n\r\n");

	// la write riceve un file descriptor fd (in questo caso corrisponde a s)
	// Un puntatore ad un array di caratteri (request)
	// la dimensione del buffer (o array di carattery) (strlen(request)))
	// se -1 da errore
	if ( -1 == write(s,request,strlen(request))){perror("write fallita"); return 1;}

bzero(h,sizeof(struct header)*100);

	statusline = h[0].n=response;

	for( j=0,k=0; read(s,response+j,1);j++){

			if(response[j]==':' && (h[k].v==0) ){

				response[j]=0;

				h[k].v=response+j+1;
			}

			else if((response[j]=='\n') && (response[j-1]=='\r') ){

				response[j-1]=0;

				if(h[k].n[0]==0) break;

				h[++k].n=response+j+1;
			}
	}

	entity_length = -1;

	printf("Status line = %s\n",statusline);

	for(i=1;i<k;i++){

		// verifico se la Content-Length è presente o meno nello header
		if(strcmp(h[i].n,"Content-Length")==0){

			// se la Content-Length è presente assegno il valore della lunghezza della Content-Length
			entity_length=atoi(h[i].v);

			// Stampo la lunghezza della Content-Length che corrisponde alla entity_length
			printf("* (%d) ",entity_length);
		}

		printf("%s ----> %s\n",h[i].n, h[i].v);
	}

	// se la Content-Length NON è presente assegno il valore 1000000 alla entity_length
	if(entity_length == -1) entity_length=1000000;

	entity = (char * ) malloc(entity_length);

	for(j=0; (t=read(s,entity+j,entity_length-j))>0;j+=t);
	//if ( t == -1) { perror("Read fallita"); return 1;}
	printf("j= %d\n",j);

	for(i=0;i<j;i++) printf("%c",entity[i]);
	}
}
