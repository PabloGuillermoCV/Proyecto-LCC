#include<stdlib.h>
#include<math.h>

/*
 * Método que concatenará dos cadenas de numeros
*/
uint16_t concatenar(uint8_t FP, uint8_t Off){
	uint16_t ret = 0x00;
	int i = 10;
	ret = FP * 100000000;
	while(off > 0){
		ret = ret + ((Off % 10)*i);
		i = i*10;
		Off = Off / 10;
	}
	return ret;
}

/*
* Intento concatenar dos numeros de 8 bits y crear uno de 16 bits
* valor Esperado: nf = 1101001000000000
*/
int main(){

	uint8_t n1 = 11010010b;
	uint8_t n2 = 00000000b;
	uint16_t nf = concatenar(n1,n2);

	printf("numero original 1 y 2 respectivamente: %d, %d \n concatenacion: %d\n", n1,n2,nf);

	return 0;
}