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

// Per testarlo con il server authentication avviarli entrambi
// Questo ip è quello per host locale
// e la porta inserita successivamente addr.sin_port = htons(PORT);
// è quella del server
unsigned char targetip[4] = { 127,0,0,1 };
//unsigned char targetip[4] = { 172,217,21,68 };
char request[5000],response[10000];

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

  // puntatole a username e password in chiaro
  char * userPass;

  s_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (s_socket == -1){

    tmp = errno;

  	perror("Socket fallita");
  	printf("i=%d errno=%d\n",i,tmp);
  	return 1;

  }


addr.sin_family = AF_INET;
addr.sin_port = htons(12121);
addr.sin_addr.s_addr = *(unsigned int*)targetip;

if (connect(s_socket,(struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {

	perror("Connect fallita");

}
//printf("%d\n", s_socket);

// password e username in chiaro nel formato username:password
userPass = "nome:matricola";
// dove salvare la codifica in base64 dell'username e pass
char encoded[100];
//printf("%s\n", h[authorIndex].v+6);
// Codifico in base64 username e pass e li posiziono in encoded
base64enc(userPass, strlen(userPass), encoded);
// GET con autorizzazione
sprintf(request, "GET /disp1.html HTTP/1.0\r\nAuthorization: Basic %s\r\n\r\n", encoded);

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

  printf("%s ----> %s\n",h[i].n, h[i].v);

}

if(entity_length == -1) entity_length = 1000000;

entity = (char * ) malloc(entity_length);

for(j = 0; (t = read(s_socket, entity + j, entity_length - j)) > 0; j += t);
printf("j= %d\n", j);

for(i = 0; i < j; i++) printf("%c", entity[i]);

}
