#include<stdlib.h>

void modificar(int arr[]){

	int i = 0;
	for(i = 0; i < 3; i++){
		arr[i] = arr[i] + 10;
	}

}


//Probar que al pasar un arreglo por parametro de la forma en que lo paso, los cambios se mantienen aun fuera de la funcion
/*Valores esperados
	Iniciales [3 ,8, 10] 
	Finales [13,18,20]
*/
int main(){

	int vector[] = {3,8,10};
	printf("Valores iniciales del arreglo: %d ,%d, %d", vector[0], vector[1], vector[2]);
	modificar(vector);
	printf("Valores finales del arreglo: %d ,%d, %d", vector[0], vector[1], vector[2]);
}