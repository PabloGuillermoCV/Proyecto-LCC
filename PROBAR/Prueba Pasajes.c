

#define __STDC_FORMAT_MACROS
#include<stdlib.h>
#include<math.h>
#include<inttypes.h>
#include<stdint.h>

uint16_t DecimalABinario(int n){
	uint16_t ret = 0x00; //000000000000000 en binario usando representación en Hexa
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
int BinarioADecimal(uint16_t n){
	int rem,decimal,base = 1;
	decimal = 0;
	rem = 0;
	while(n > 0){
		rem = n % 10;
		decimal = decimal + rem * base;
		n = n / 10;
		base = base * 2;
	}

	return decimal;
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
	printf("numero original: %d\n numero en binario: %"PRIu16"\n numero de binario a decimal: %d\n", numP,ret,numB);
	return 0;
}