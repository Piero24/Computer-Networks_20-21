#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

int tmp;

struct header{
char * n;
char * v;
}h[100];

int main()
{
struct sockaddr_in addr,remote_addr;
int i,j,k,s,t,s2,len;
int c;
FILE * fin;
int yes=1;
char * method, *path, *ver;
char request[5000],response[10000];
s =  socket(AF_INET, SOCK_STREAM, 0);
if ( s == -1 ){ perror("Socket fallita"); return 1; }
// Indirizzo ipv4
addr.sin_family = AF_INET;
// Port del server
addr.sin_port = htons(8033);
// Specifico che non voglio associare nessun indirizzo ip specifico al socket
addr.sin_addr.s_addr = 0;
t= setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
if (t==-1){perror("setsockopt fallita"); return 1;}

//
// Se fallisce ritorna errore
if ( bind(s,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {perror("bind fallita"); return 1;}
// indico che il socket che utilizzerà il server è un socket passivo
// ossia un socket che accetta le richieste in entrata
// e in questo caso può accettare 5 connessioni in entrata da client
// Se fallisce ritorna errore
if ( listen(s,5) == -1 ) { perror("Listen Fallita"); return 1; }

len = sizeof(struct sockaddr_in);

while(1){

// La accept crea un nuovo socket con port e indirizzo di chi ha iniziato
// s2 quindi è il socket
s2 =  accept(s, (struct sockaddr *)&remote_addr,&len);
// Se fallisce ritorna errore
if ( s2 == -1 ) { perror("Accept Fallita"); return 1;}
// Leggo la richiesta arrivata nella connessione
t = read(s2,request,4999);
// Se fallisce ritorna errore
if ( t == -1 ) { perror("Read fallita"); return 1;}
// Imposto il terminatore nel buffer della request
request[t]=0;
// Stampo a schermo la richiesta
printf("%s",request);

// I seguenti cicli fanno i parsing degli header per recuperare
// le info della richiesta che è stata effettuata nel confronti del server
method = request;

for(i=0;request[i]!=' ';i++){} request[i]=0; path = request+i+1;

for(i++;request[i]!=' ';i++); request[i]=0; ver = request+i+1;

for(i++;request[i]!='\r';i++); request[i]=0;
// Stampano le informazioni appena parsate
printf("method=%s path=%s ver=%s\n",method,path,ver);

if ((fin = fopen(path+1,"rt"))==NULL){

	sprintf(response,"HTTP/1.1 404 Not Found\r\n\r\n");

	write(s2,response,strlen(response));

	} else {

			sprintf(response,"HTTP/1.1 200 OK\r\n\r\n");

			write(s2,response,strlen(response));

			while ( (c = fgetc(fin)) != EOF )

				write(s2,&c,1);

			fclose(fin);
		}
			close(s2);
}
}

/*
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
		if(strcmp(h[i].n,"Content-Length")==0){
			entity_length=atoi(h[i].v);
			printf("* (%d) ",entity_length);
		}
		printf("%s ----> %s\n",h[i].n, h[i].v);
	}
	entity_length=22000;
	if(entity_length == -1) entity_length=1000000;
	entity = (char * ) malloc(entity_length);
	for(j=0; (t=read(s,entity+j,entity_length-j))>0;j+=t);
	//if ( t == -1) { perror("Read fallita"); return 1;}
	printf("j= %d\n",j);

	for(i=0;i<j;i++) printf("%c",entity[i]);
	}
*/
