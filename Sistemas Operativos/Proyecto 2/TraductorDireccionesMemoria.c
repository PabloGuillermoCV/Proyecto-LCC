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
		Lo que hago aquí es, dado el número de 8 bits, pasarlo a decimal y acceder al arreglo directamente a la componente que apunte el numero convertido (preguntar)
			Cuando se implante el TLB, se debe agregar que primero se busque en el TLB y en caso de un Miss, bajar a la Tabla, si bajo a la Tabla, una vez encontrado el frame 
			debo agregarlo al TLB para futuro uso
	3) Obtener la Dirección Física
		Hecho todo el trámite, deberia de terminar con al dirección física a devolver (preguntar que se hace acá, si realmente debo acceder a algún lado interno de la PC
		a por la dirección Física o si el FramexOffset es ya la dirección física)
*/

/*
 * Dado el numero inicial, pasarlo a binario
*/
uint16_t DecimalABinario(int n){
	uint16_t ret = 0x00; //000000000000000


	return ret;

}

/*
 * Dado un Numero binario, lo paso a Decimal
*/
int BinarioADecimal(uint8_t n){
	int i = 7; //acordarse que las posiciones van de 0 a n-1 cuando haciamos las conversiones en Orga
	int ret = 0;
	int aux = 0;
	while(i >= 0){
		//Agarrar bit por bit y pasarlo a decimal
		aux = n >> i; //Bit shifting, lo que intento obtener el la compenente i-esima del numero binario
		ret = ret + (/*2*Bit del numero elevado a la i*/);

	}

	return ret + 1; //Esto lo teniamos que hacer en Organización si mal no recuerdo... 
}



/*
 * Dado un numero de página, buscar el numero de frame correspondiente en la Tabla de Páginas
*/
int BusquedaTabla(uint8_t PN){
	int Nro = BinarioADecimal(PN);

	return Nro;
}

/*
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Mi idea es usar un algoritmo FIFO para reemplazar, preguntar
*/
void Reemplazar(){

}

/* Método que se encargará de agrandar el arreglod e direcciones a traducir en caso que sea necesario
 *
*/
void agrandarArreglo(){

}

//EL código de lectura de archivos hayq eu probarlo por separado en algún otro lado, hay que probar quye esa cosa ande si la vamos a agarrar del Proyecto 1

void main(){

	int Direcciones [10]; //Arreglo donde guardaremos las direcciones, valor inicial para 10 direcciones, se debe poder agrandar
	int TablaPaginas [256]; //Tabla de Páginas OJO, va de 0 a 255! cuando accedamos hay que restarle 1 al numero con el que se accederá!
	int TLB [16][2]; //TLB
	FILE *Direcc; //Archivo de entrada del proyecto

	uint16_t num;

}