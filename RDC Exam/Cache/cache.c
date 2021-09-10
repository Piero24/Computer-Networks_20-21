#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


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

//unsigned char targetip[4] = {216, 58, 213, 228};
//
unsigned char targetip[4] = {127,0,0,1};
char request[5000], response[10000];


//
//
int q;
int last_modified;
int date_now;
double seconds;
FILE *fp;
int flag = -1;
FILE *fptr;
char file_name_buff[1000];


// Nome del sito di cui voglio fare il caching
//char name[] = "www.google.com";
char name[] = "localhost:25000/";

char * convert() {

  int len = strlen(name);
  char *str = malloc(len);

  for(int a = 0; a < len; a++) {

    str[a] = (name[a] == '/')?'_':name[a];

  }

  return str;
}


// Questo metodo è la parte finale del file di base con alcune modifiche indicate.
// E stato scritto in un metodo per non far allungare il codice e renderlo più leggibile.
// La sua funzione è quella di scoprire l'entity lenght e inserire l'entity body in entity
// ritornando il puntatore ad entity
char * f_funct(int entity_length, char *statusline, int k, struct header h[], char *entity, int s_socket) {

  entity_length = -1;

  printf("Status line = %s\n", statusline);

  for(i=1; i<k; i++) {

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

  //for(i = 0; i < j; i++) printf("%c", entity[i]);

  return (entity);
}





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
  sprintf(request, "GET / HTTP/1.0\r\nHost:%s\r\n\r\n", name);

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


  // Assegno un valore nullo al puntatore
  // Se non verrà trovato il valore di last_modified => questo valore non verrà cambiato
  last_modified = -1;
  date_now = -1;

  // Verifico per tutti gli elementi nella struct header appena scaricata
  // se è presente quello contenente la data dell'ultima modifica del File
  // ovvero last-modified
  for (q = 0; (q < 10) && (h[q].n != NULL); q++) {

    // se presente
    if (strcmp(h[q].n, "Last-Modified") == 0) {

      // assegno il puntatore al al suo valore presente nella struct header
      last_modified = q;
      printf("Last-Modified value: %s\n", h[q].v);

    }
  }

  // Creo il buffer e la sua dimensione che conterrano la data
  // presente nel file di cache
  size_t bufsize = 1000;
  char buffer[bufsize];

  char *ccc = convert();
  //printf("%s", ccc);

  // Appendo la porzione di stringa e l'indiizzo da far puntare al puntatore
  // file_name_buff. Mi serve come nome dei file da aprire
  sprintf(file_name_buff,"./cacheC/%s.txt", ccc);

  // Apro il file e prendo la data nella prima riga che posiziono nel buffer
  // di nome buffer e dimensione bufsize
  // Se non esiste salta questa parte e non mettendo il flag a 1 al passo successivo
  // saprà già che non può leggere nulla dal file ma deve prendere l'entity dalla rete
  fp = fopen(file_name_buff, "r");
  if(fp != NULL) {

    fgets(buffer, bufsize, fp);
    printf("%s", buffer);
    flag = 1;
    fclose(fp);
   }

  // se la data nel file è successiva a quella di last-modified
  // Altrimenti richiedo l'entity body lo stampo e lo salvo nel file dalla seconda riga in poi
  // Come prima riga metto la data odierna di download
  time_t now = time(0);
	struct tm tm = *gmtime(&now);
  struct tm tm2 = *gmtime(&now);

  // Confronta la data del foglio con quella presa dagli header
  // Ma devo covertire entrambe le date prese essendo che sono entrambe in formato stringa
  // e la comparazione va fatta tra 2 tipi di date

  //==================================================================================================== DA SISTEMARE
  // bisogna cambiare lastmodif perchè è intero e invece deve arrivargli la stringa
  strptime(buffer, "%a, %d %b %Y %H:%M:%S %Z", &tm);

  if (last_modified != -1) {

    strptime(h[last_modified].v, "%a, %d %b %Y %H:%M:%S %Z", &tm2);
    seconds = difftime(mktime(&tm), mktime(&tm2));

  } else {

    seconds = -1.0;
  }
  //printf("%f", seconds);

  // Se seconds > 0 vuol dire che la data nel file è maggiore di quella di last-Modified
  if ((seconds > 0) && (flag != -1)) {

    printf("\nStampo quello che è nel file\n\n");

    // Apre il file e legge la prima riga
    // Essa verrà scartata e non sarà stampata a schermo
    fp = fopen(file_name_buff, "r");
    fgets(buffer, bufsize, fp);

    // Stampa tutte le successive righe del file a schermo
/*
==========================================================================================>>>>> DA DECOMMENTARE POI
    while (fgets(buffer, bufsize, fp)) {

      printf("%s", buffer);

    }
*/
    fclose(fp);

  } else {

    printf("\nStampo roba nuova e la scrivo nel file\n\n");
    // ritorna il puntatore ad entity contenente l'entity body
    entity = f_funct(entity_length, statusline, k, h, entity, s_socket);
    // Apre il file in cui scriverlo
    fptr = fopen(file_name_buff, "w");
    // Prende il tempo in cui il file viene salvato
    time_t ltime = time(0);
    // Scrive nel file alla prima riga la data e dalla successiva l'entity body
    fprintf(fptr, "%s%s", ctime(&ltime), entity);
    // Stampa a schermo l'entity body
    //==========================================================================================>>>>> DA DECOMMENTARE POI
    //for(i = 0; i < j; i++) printf("%c", entity[i]);
    fclose(fptr);
  }
}
