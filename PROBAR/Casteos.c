#include<stdlib.h>
#include<math.h>
#include <bool.h>

int main(){

	int A = 10;
	uint16_t ret = 0x00;
	ret = (uint16_t)A;
	int B = (int)ret;

	printf("Num original: %d, casteo a 16 bits: %d, casteo de 16 bits devuelta a int: %d \n", A,ret,B);
}