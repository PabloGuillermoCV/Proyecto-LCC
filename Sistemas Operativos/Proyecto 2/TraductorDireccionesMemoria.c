#include<stdlib.h>


/*PASOS A SEGUIR:
	
	0) Leer el archio y obtener todas las direcciones lógicas a traducir
	1) Obtener la Dirección Lógica 
		Lo que debo hacer es, dado el numero entero, Pasarlo a Base 2 
		y de ahí usar Enmascarado de Bits para separar el numero de 16 bits resultante en dos secuencias de 8 bits
		que serán el Numero de Página y el Offset
	2) Acceder con el Número de Página a la Tabla de Páginas/TLB
		En primera Instancia, una vez que se el numero de página, accedo a la Tabla de Páginas y busco el Frame correspondiente
		Una vez implementada la Tabla de Páginas (arreglo de 256 componentes) y probada, se debe añadir el TLB el cual es un arreglo de 16x2
		OJO, para el TLB es necesario implementar un algoritmo de reemplazo (FIFO, LRU, etc)
	3) Obtener la Dirección Física
		Hecho todo el trámite, deberia de terminar con al dirección física a devolver
*/

/*
 * Dado el numero inicial, pasarlo a binario
*/
uint16_t DecimalABinario(int n){
	uint16_t ret = 0x00; //000000000000000

	return ret;

}



/*
 * Dado un numero de página, buscar el numero de frame correspondiente en la Tabla de Páginas
*/
int BusquedaTabla(){
	int Nro = 0;

	return Nro;
}

/*
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Mi idea es usar un algoritmo FIFO para reemplazar, preguntar
*/
void Reemplazar(){

}

void main(){

	int Direcciones [10]; //Arreglo donde guardaremos las direcciones, valor inicial para 10 direcciones, se debe poder agrandar
	int TablaPaginas [256]; //Tabla de Páginas
	int TLB [16][2]; //TLB
	FILE *Direcc; //Archivo de entrada del proyecto

	uint16_t num;

}