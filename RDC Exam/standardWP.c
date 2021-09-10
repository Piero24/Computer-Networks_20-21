#include <sys/types.h>          /* See NOTES */
#include <signal.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

//
struct hostent *he;
//
struct sockaddr_in local, remote, server;

// Buffer contenenti le request
char request[2000], response[2000], request2[2000], response2[2000];
// Puntatori
char * method, *path, *version, *host, *scheme, *resource,*port;

// Struttura contenente gli header
struct headers {
char *n;
char *v;
}h[30];

int main() {


	// Puntatore al file
	FILE *f;
	char command[100];
	int i,s,t,s2,s3,n,len,c,yes=1,j,k,pid;

	// chiamata a sistema che apre una comunicazione e restituisce un INT,
	// che è un File Descriptor ovvero l’indice della tabella con tutto ciò
	// che serve per gestire la comunicazione
	// int socket(int domain, int type, int protocol);
	s = socket(AF_INET, SOCK_STREAM, 0);

	if ( s == -1) {

		// Printa testo di errore
		perror("Socket Fallita\n");
		return 1;
	}

	// Indirizzo ipv4 del server che attende la richiesta
	local.sin_family = AF_INET;
	// Port del server
	// host to network short (htons)
	local.sin_port = htons(12101);
	// Puntatore all'array contenente l'indirizzo ip del server
  // Essendo io il serve è zero
	local.sin_addr.s_addr = 0;

	// Indico al sistema operativo che in caso di riavvi ecc
  // Voglio poter riutilizzare la stessa porta usata in precedenza
	setsockopt(s,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	// Connette il socket indicando il tipo (AF_INET), la porta da usare e in questo caso
  // Indico che sono un serve con addr.sin_addr.s_addr = 0
	t = bind(s, (struct sockaddr *) &local, sizeof(struct sockaddr_in));

	if ( t == -1) {
		// Printa testo di errore
		perror("Bind Fallita \n");
		return 1;
	}
	// Dico al socket che avrà un backlog max di 10 cliente
  // Ovvero max 10 tentativi diversi di accedere alla risorsa
	// mentre sto soddisfando la prima richiesta
	t = listen(s, 10);

	if ( t == -1) {

		// Printa testo di errore
		perror("Listen Fallita \n");
		return 1;
	}

	while( 1 ){

		// "Svuota" il puntatore al file
		f = NULL;
		// Come parametro per la creazione del socket tra parte server ed il client
		// che fa la richiesta isa un protocollo di tipo AF_INET
		remote.sin_family = AF_INET;

		// Lunghezza della struttura che ricevo dal client
		len = sizeof(struct sockaddr_in);
		// crea un socket "figlio" della socket s e scrive nella mia strutturra
    // remote_addr le informazioni del client e collega questo
    // nuovo socket a quello del cliente
		s2 = accept(s, (struct sockaddr *) &remote, (unsigned int *)&len);
		// Crea un nuovo thread per ogni connessione se il fork ritorna 0
		// allora è il processo figlio e prosegu al codice seguente
		// altrimenti è il processo genitore e vai all'inizio del ciclo
		if(fork()) continue;

		if (s2 == -1) {

			// Printa testo di errore
			perror("Accept Fallita\n");
			return 1;
		}

		// Legge i vari header in arrivo dal client
		// e li dispone per nome e valore nella struct header
		j = 0;
		k = 0;
	  h[k].n = request;

		while(read(s2, request + j, 1)){

			if((request[j] == '\n') && (request[j-1] == '\r')) {

				request[j-1] = 0;
				if (h[k].n[0] == 0) break;
				h[++k].n = request + j + 2;

			}
			if (request[j] == ':' && (h[k].v == 0) && k ){

				request[j] = 0;
				h[k].v = request + j + 1;

			}

		j++;

		}

		// Stampa la request
		printf("%s",request);
		// punta all'inizio della request
		method = request;

		// Con questo ciclo appena trova uno spazio tra i caratteri della prima richiesta
    // esce dal ciclo e sostituisce alla spazio il valore zero, così da avere il tipo di richiesta separato
    // dal resto che viene messo in path
		for(i = 0; (i < 2000) && (request[i] != ' '); i++){} request[i] = 0;
		path = request + i + 1;

		// Con questo ciclo appena trova uno spazio tra i caratteri della prima richiesta
    // esce dal ciclo e sostituisce alla spazio il valore zero, così da avere il tipo di richiesta separato
    // dal resto che viene messo in vet
		for(   ;( i < 2000) && (request[i] != ' '); i++){} request[i] = 0;
		version = request + i + 1;

		printf("Method = %s, path = %s , version = %s\n", method, path, version);

		if(!strcmp("GET", method)){
			//  http://www.google.com/path
			scheme = path;
			for(i = 0; path[i] != ':'; i++){} path[i] = 0;
			host = path + i + 3;
			for(i = i + 3; path[i] != '/'; i++){} path[i] = 0;
			resource = path + i + 1;

			printf("Scheme=%s, host=%s, resource = %s\n", scheme, host, resource);
			// Trova l'indirizzo ip del server dove è posizinata la risorsa
			// richiesta. (Se cerco: www.google.com mi restituirà l'indirizzo ip
			// del server di google)
			he = gethostbyname(host);
			//he = gethostbyname("www.example.com");

			// Se non trova l'indirizzo ip del server mi da errore
			if (he == NULL) {

				printf("Gethostbyname Fallita\n");
				return 1;
			}

			// Stampa l'indirizzo ip del server alla quale inoltrare la richiesta
			printf("Server address = %u.%u.%u.%u\n", (unsigned char ) he->h_addr[0],(unsigned char ) he->h_addr[1],(unsigned char ) he->h_addr[2],(unsigned char ) he->h_addr[3]);


			s3 = socket(AF_INET, SOCK_STREAM, 0);

			if(s3 == -1){

				perror("Socket to server fallita");
				return 1;
			}

			// Indirizzo ipv4 del client che fa la richiesta
			server.sin_family = AF_INET;
			// Port del client
			// host to network short (htons)
			server.sin_port = htons(80);
			// Puntatore all'array contenente l'indirizzo ip del server
		 	server.sin_addr.s_addr =* (unsigned int *) he -> h_addr;

			// connect(): è una chiamata a sistema per connettersi al socket riferito dal
			// file descriptor sockfd all’indirizzo specificato da addr. addrlen specifica
			// la lunghezza dell’indirizzo.

			// sockaddr è generica

			// sockaddr_in è dedicata all' IPv4 cioè per essere usata con
			// l'Internet Protocol (TCP o UDP per il trasporto).
			t = connect(s3, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

			if(t == -1){

				// Printa testo di errore
				perror("Connect to server fallita");
				return 1;
			}


			sprintf(request2, "GET /%s HTTP/1.1\r\nHost:%s\r\nConnection:close\r\n\r\n", resource, host);

			// la write riceve un file descriptor fd (in questo caso corrisponde a s)
			// Un puntatore ad un array di caratteri (request)
			// la dimensione del buffer (o array di carattery) (strlen(request)))
			// se -1 da errore
			write(s3, request2, strlen(request2));

			while((t = read(s3, response2, 2000)) != 0) {

				write(s2, response2, t);

			}

			shutdown(s3, SHUT_RDWR);
			close(s3);
		}
		else if(!strcmp("CONNECT", method)) { // it is a connect  host:port

			host = path;

			for(i = 0; path[i] != ':'; i++){} path[i] = 0;

			port = path + i + 1;

			printf("host:%s, port:%s\n", host, port);
			printf("Connect skipped ...\n");
			// Trova l'indirizzo ip del server dove è posizinata la risorsa
			// richiesta. (Se cerco: www.google.com mi restituirà l'indirizzo ip
			// del server di google)
			he = gethostbyname(host);

			// Se non trova l'indirizzo ip del server mi da errore
			if (he == NULL) {

				// Printa testo di errore
				printf("Gethostbyname Fallita\n");
				return 1;
			}
			// Stampa l'indirizzo ip del server alla quale connettersi
			printf("Connecting to address = %u.%u.%u.%u\n", (unsigned char ) he->h_addr[0],(unsigned char ) he->h_addr[1],(unsigned char ) he->h_addr[2],(unsigned char ) he->h_addr[3]);

			s3 = socket(AF_INET,SOCK_STREAM,0);

			if(s3 == -1){
				// Printa testo di errore
				perror("Socket to server fallita");
				return 1;
			}

			server.sin_family = AF_INET;
			server.sin_port = htons((unsigned short)atoi(port));
		 	server.sin_addr.s_addr =* (unsigned int*) he->h_addr;

			t = connect(s3, (struct sockaddr *)&server, sizeof(struct sockaddr_in));

			if(t==-1){

				// Printa testo di errore
				perror("Connect to server fallita");
				exit(0);
			}

			sprintf(response, "HTTP/1.1 200 Established\r\n\r\n");
			write(s2, response, strlen(response));
				// <==============

			if(!(pid = fork())){ //Child


				while((t = read(s2, request2, 2000)) != 0){

					write(s3, request2, t);
				//printf("CL >>>(%d)%s \n",t,host); //SOLO PER CHECK
					}
				exit(0);
				} else { //Parent

					while((t = read(s3, response2, 2000)) != 0){
						write(s2, response2, t);
						//printf("CL <<<(%d)%s \n",t,host);
					}

				kill(pid, SIGTERM);
				shutdown(s3, SHUT_RDWR);
				close(s3);

				}
			}

			shutdown(s2, SHUT_RDWR);
			close(s2);
			exit(0);

		}
}
