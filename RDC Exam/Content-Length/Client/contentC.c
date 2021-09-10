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

unsigned char targetip[4] = { 172,217,21,68 };
char request[5000],response[10000];

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

sprintf(request, "GET / HTTP/1.0\r\nHost:www.google.com\r\n\r\n");

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
// Inizializzo la verifica della content-lenght
int contentL = 0;
for(i = 1; i < k; i++) {

  if(strcmp(h[i].n, "Content-Length") == 0) {

    entity_length = atoi(h[i].v);
    //printf("* (%d) ",entity_length);
    // Pongo ugale a 1 se esiste la content-lenght
    contentL = 1;

  }

  printf("%s ----> %s\n",h[i].n, h[i].v);

}

if(entity_length == -1) entity_length = 1000000;

entity = (char *)malloc(entity_length);

// Se esiste
if(contentL) {
    // Legge tutti i caratteri in arrivo dell'entity body sulla base
    // della quantità in arrivo data dalla content-lenght
    // e li mette in entity
    for(j=0; read(s_socket,entity+j,entity_length-j)>0;j+=t);
}
printf("j= %d\n", j);

for(i = 0; i < j; i++) printf("%c", entity[i]);

free(entity);
}
