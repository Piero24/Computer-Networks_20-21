#include <stdio.h>

//Esercizio 1: trovare se l'architettura è little o big endian

int isBigOrLittle() {
	//ritorna 0 se little endian, 1 se big endian, -1 se ci sono problemi

	//riservo aree di memorie
	char * pc; //puntatore che scandisce la memoria a byte
	short int * pi;
	short int i = 1;

	//associo le aree di memoria a due puntatori di tipo diverso
	pi = &i;
	pc = (char*) &i;

	//se il primo byte dell'area allocata (short int -> 2 byte) contiene il dato '1' e il secondo '0'
	if ( *pc == i && *(pc + 1) == 0 ) {
		return 0; //little endian
	}
 
	//se il secondo byte dell'area allocata (short int -> 2 byte) contiene il dato '1' e la prima '0'
	if ( *pc == 0 && *(pc + 1) == 1) {
		return 1; //big endian
	}

	return -1;
}

//Esercizio 2: convertire unsigned short int da little endian e big endian

/*Inverte i byte dell'unsigned short int (2) e ritorna il nuovo valore memorizzato, -1 se ci sono problemi*/
unsigned short int endianConversion(unsigned short int x) {
	unsigned short int * ps = &x;	
	char * pc = (char*) ps; //scandisco la memoria a byte
	unsigned short int expected = *(pc)*256 + *(pc+1)*1; //valore aspettato post conversione
	unsigned int start = *ps; //valore iniziale

	//byte temporaneo sove salvare il dato da spostare
	char temp;

	//indico il tipo di conversione che verrà eseguita
	if (isBigOrLittle() == 0) {
		printf("\nIs little Endian\n");
	//scambio i valori dei due byte
	temp = *pc; //memorizzo il primo byte in un temp
	*pc = *(pc+1);
	*(pc+1) = temp;
	}
	else {
		printf("\nIs Big Endian\n");
		// Nessuna Conversione 
	}


	if (*ps == expected) {
		//se il valore e' quello previsto proseguo
		printf("\nStart value: ");
		printf("%d", start);
		printf("\nExpected value: ");
		printf("%d", expected);
		printf("\nFinal value: ");
		printf("%d", *ps);
		printf("\n\n");

	return *ps;
	}
	else {
		//se il valore non è quello aspettato mi fermo
		printf("\nI have not converted the unsigned short int from Little Endian to Big Endian, an error has occured\n\n");
		return -1;
	}

	return -1;
}

int main(void) {
	//verifico di essere in big endiano o little endian
	
	printf("\n\n          Esercizi 2021-03-20\n          Big o Little Endian e conversione\n\n");
	printf("*********************************************************\n\n");
	printf("Is this architecture Big Endian or Little Endian?\n\n");

	short int result = isBigOrLittle();

	if (result == 0) {
		printf("Little endian\n\n\n");
	}
	else if (result == 1) {
		printf("Big Endian\n\n\n");
	}
	else {
		printf("An error has occured\n\n\n");
	}

	//converto l'unsigned short int da un'impostazione all'altra (big->little o little->big)
	
	printf("Endian conversion\n\n");
	unsigned short int s;
	printf("Insert a value: ");
	scanf("%hu", &s);
	printf("\n");

	printf("Il valore convertito è : %d\n",endianConversion(s));

	return 0;
}
