#include<stdlib.h>
#include<math.h>

uint16_t DecimalABinario(int n){
	uint16_t ret = 0x00; //000000000000000
	int aux;
	int i = 1; //para ir sumando 1, 10, 100, etc
	while(n > 0){
		aux = n % 2;
		n = n / 2;
		ret = ret + (aux * i); 
		i = i * 10; 
	}

	return ret; //Dudo de si esto esta bien.

}

/*
 * Dado un Numero binario, lo paso a Decimal, uso método del producto
*/
int BinarioADecimal(uint8_t n){
	int i = 7; //acordarse que las posiciones van de 0 a n-1 cuando haciamos las conversiones en Orga
	int ret = 0;
	int aux = 0;
	while(i >= 0){
		//Agarrar bit por bit y pasarlo a decimal
		aux = n % 10;
		ret = ret + (2*pow(aux,i)); /*2*Bit del numero elevado a la i*/
		n = n / 10;
		i--;

	}

	return ret + 1; //Esto lo teniamos que hacer en Organización si mal no recuerdo... (Representaciones)
}

/*
* intento ver si las transofrmaciones de decimal a binario y viceversa funcionan:
* Valores epserados:
* 	numP = 10
*	ret = 0000000000001010 
*	numB = 10
*/
int main (){

	int numP = 10;
	uint16_t ret = DecimalABinario(numP);
	int numB = BinarioADecimal(ret);
	printf("numero original: %d\n numero en binario: %d\n numero de binario a decimal: %d\n", numP,ret,numB);
	return 0;
}