#include<stdio.h>

int main()
{
char a[10];
int i;
int x;
//scanf("%d",&x);
//printf("x=%d\n",x);
printf("Sono prima dell'errore");
for(i=0; i<200000000;i++) {x=x+i;}
printf("%d",x);
for(i=1000000; i<2000000;i++)
	a[i]=255;

printf("Sono dopo l'errore");
}
