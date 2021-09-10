#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>

int tmp;
int main()
{
int i,s;
for(i=0;i<10000;i++){
s =  socket(AF_INET, SOCK_STREAM, 0);
if ( s == -1 ){
	tmp=errno;
	perror("Socket fallita");
	printf("i=%d errno=%d\n",i,tmp);
	return 1;
	}
}
printf("%d\n",s);
}

