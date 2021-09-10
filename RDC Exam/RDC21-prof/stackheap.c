#include<stdlib.h>
#include<stdio.h>

int input = 5; //Static

int fattoriale(int in){
	int * c; //Stack
	printf("Pointer size : %ld\n",sizeof(c));
	c = malloc(sizeof(int)); //Heap memory allocation
	printf("in=%d Stack (c) = %lx, Heap (malloc) = %lx\n",in, (unsigned long)&c , (unsigned long) c );
	*c = in;
	if (*c == 1) return 1;
	return fattoriale(*c-1)*(*c);
}

int main()
{
	printf("sizeof(int)=%ld\n",sizeof(int));
	printf("static area: %lx\n",(long int) &input);
	printf("%d\n",fattoriale(input));
}
