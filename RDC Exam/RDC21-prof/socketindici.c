#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdio.h>


int main()
{
int s;
s =  socket(AF_INET, SOCK_STREAM, 0);
printf("%d\n",s);
s =  socket(AF_INET, SOCK_STREAM, 0);
printf("%d\n",s);
s =  socket(AF_INET, SOCK_STREAM, 0);
printf("%d\n",s);
s =  socket(AF_INET, SOCK_STREAM, 0);
printf("%d\n",s);

}

