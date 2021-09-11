#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;

int main() {
struct sockaddr_in addr;
int i,s,t;
// sono i 2 buffer per la richiesta e la risposta
char request[5000],response[1000000];

// Array contenete l'indirzzo ip del server
unsigned char targetip[4] = { 216, 58 ,213,100 };


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
sprintf(request,"GET / \r\n");

// la write riceve un file descriptor fd (in questo caso corrisponde a s)
// Un puntatore ad un array di caratteri (request)
// la dimensione del buffer (o array di carattery) (strlen(request)))
// se -1 da errore

if ( -1 == write(s,request,strlen(request))){perror("write fallita"); return 1;}

// la read riceve un file descriptor fd (in questo caso corrisponde a s)
// Un puntatore ad un array di caratteri (responce)
// la dimensione del buffer (o array di carattery) (dimensione della responce)
// Viene tralasciata l'ltima cella nella dimensione della responce perchè la uso per il terminatore

while((t=read(s,response,999999))>0){

//for(i=0;i<t;i++) printf("%c",response[i]);

	// é il carattere che indica la terminazione della stringa

	response[t]=0;

	//printf("%s",response);
	printf("\nt = %d\n",t);
	}
	// se la t fallisce e quindi -1 mi dice che c'è lerrore
	if ( t == -1) { perror("Read fallita"); return 1;}

}
