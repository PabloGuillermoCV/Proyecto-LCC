#include<stdlib.h>
#include<math.h>


//Probar si las mascaras funcionan correctamente
//valores esperados:
/*
* High = 11011001
* Low = 10101101
*/
int main(){

	uint16_t num = 1101100110101101b;
	uint16_t MaskI = 0000000011111111b; //Máscaras que usaré para obtener el Page Number y el Offset de la Dirección Lógica
	uint16_t MaskS = 1111111100000000b; //Recordar que la dirección Lógica es de 16 bits, necesito comparaciones en 16 bits 
	uint8_t High = (numCopy & MaskS) / 100000000; 
	uint8_t Low = ((numCopy & MaskI) << 8 ) / 100000000;  //Uso la máscara, como me quedo con los 8 LSB, los muevo para arriba y corto

	printf("numero original: %d\n, mascara Inferior: %d\n, mascara Superior: %d\n, resultado de aplicar la mascara superior: %d\n, resultado de aplicar la mascara Inferior: %d\n"
			,num,MaskI,MaskS,High,Low);
	return 0;
}