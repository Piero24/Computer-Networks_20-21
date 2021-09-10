#include <stdio.h>

//gcc [programName].c -o programName
//./programName

//scp -r 1143259@88.80.187.84:/RDC21 /Users/pietrobon/Desktop
//scp 1143259@88.80.187.84:/RDC21/file.txt /Users/pietrobon/Desktop

//man socket

int main(){

	//Inizializzo il puntatore p
	char *p;
	//Inizializzo la variabile d
	char d[10];

	d[0] = 'C';
	d[1] = ' I';

	printf("%c", d[0]);
	printf("%c", d[1]);

	//Stampo il valore char della variabile d
	//printf("%c", d);
	//Stampo il numero in ascii della variabile d
	//printf("%d", d);
	//Stampo il numero in esadecimale della variabile d
	//printf("%x", d);

	//Stampo l'indirizzo (in sadecimale) dove è contenuta la variabile d
	//printf("%x", &d);

	//Il puntatore p punta alla cella di memoria della variabile d
	//p = &d;

	//IL puntatore va a sovrascrivere la cella della variabile d con il valore c
	//*p = "c";

	//IL puntatore va a sovrascrivere la cella successiva alla variabile d con il valore D
	//*(p+1) = "D";

	return 0;


}
