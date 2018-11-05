#include<stdlib.h>
#include<math.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera


/*PASOS A SEGUIR:
	
	0) Leer el archivo y obtener todas las direcciones lógicas a traducir
	1) Obtener la Dirección Lógica 
		Lo que debo hacer es, dado el numero entero, Pasarlo a Base 2 
		y de ahí usar Enmascarado de Bits para separar el numero de 16 bits resultante en dos secuencias de 8 bits
		que serán el Numero de Página y el Offset
	2) Acceder con el Número de Página a la Tabla de Páginas/TLB
		En primera Instancia, una vez que se el numero de página, accedo a la Tabla de Páginas y busco el Frame correspondiente
		Una vez implementada la Tabla de Páginas (arreglo de 256 componentes) y probada, se debe añadir el TLB el cual es un arreglo de 16x2
		OJO, para el TLB es necesario implementar un algoritmo de reemplazo (FIFO, LRU, etc)
		Lo que hago aquí es, dado el número de 8 bits, pasarlo a decimal y acceder al arreglo directamente a la componente que apunte el numero convertido (preguntar)
			Cuando se implante el TLB, se debe agregar que primero se busque en el TLB y en caso de un TLB Miss, bajar a la Tabla, si bajo a la Tabla, una vez encontrado el frame 
			debo agregarlo al TLB para futuro uso
	3) Obtener la Dirección Física
		Hecho todo el trámite, deberia de terminar con al dirección física a devolver (preguntar que se hace acá, si realmente debo acceder a algún lado interno de la PC
		a por la dirección Física o si el FramexOffset es ya la dirección física)

	Sobre uint_x:
		0x.. -> el número lo represento en Hexadecimal
		0b.. -> el número lo represento en Binario
		Sin literal despues del primer cero lo estoy trabajando en Decimal
		También puedo representar el numero completo y poner el literal de representación al final
		1111b es un número binario de 4 bits, por ejemplo

	Sobre Enmascarado:
		La cosa funciona asi, yo al enmascarar, me quedo con los bits que me "Importan" al aplicar una operación AND entre el 
		Numero original en binario y un número binario arbitrario cuyas posiciones en 1 denotan QUE posiciones de bits dejaré como están

		asumamos que el numero es 101011010111111 y la máscara es 1111111100000000
		al hacer 101011010111111 & 1111111100000000 obtengo 1010110100000000

	Sobre Shifting:
		la operación >> o << denota un shifting de los bits del número a derecha o a izquierda respectivamente (igual que haciamos en Orga)
		se permite usar potencias de dos para hacer shifting de varias posiciones de una.
		LOS BITS QUE SE CAEN DEL NÚMERO SE PIERDEN
		si mi numero es 010110, hacer 010110 >> resulta en 001011

	Una cosa que estoy viendo es que uint_x NO es un tipo "standard", preguntar si su uso esta bien
	También una cosa que estoy leyendo es que al usar ">>", el numero pasa a int si o si, preguntar 
	preguntar que deberia contener la tabla de paginas, si numeros random o algo en particular

	lo que hace >> es mover los bits del numero, si muevo mal, puedo perder bits, lo que yo necesito es CORTAR los bits innecesarios 
	despues de usar la máscara, preguntar como hacer, si se puede usar /10 o %10 o si hay que usar >> pero de alguna forma que no estoy entendiendo
	Otra cosa a preguntar es CUALES son los contenidos de la tabla de páginas, si números random, si se tienen que buscar en algún lado.... 
*/

/*
 * Dado un numero en decimal, pasarlo a binario, uso método de la División
*/
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
 * Dado un numero de página, buscar el numero de frame correspondiente en la Tabla de Páginas
*/
int BusquedaTabla(uint8_t PN){
	int Nro = BinarioADecimal(PN);
	int frame = 0;
	//Lo que hago es acceder a la tabla de Paginas y buscar el valor, preguntar si pasar la tabla por puntero o como parámtero
	frame = TablaPaginas[Nro + 1];
	return frame; //NO tengo idea de como manejar el frame, preguntar
	
}

/*
 * Dado un número de Página, lo intento buscar en el TLB, en caso de no encontrarlo, debo bajar a la Tabla de Páginas
 * NOTA: Devolver -1 significa que NO se encontró el número de página en el TLB
*/
int BusquedaTLB(uint8_t PN){

}

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
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Mi idea es usar un algoritmo FIFO para reemplazar, preguntar
*/
void Reemplazar(int FP, uint8_t PN){
	int i = 0;
	

}

/* Método que se encargará de agrandar el arreglo de direcciones a traducir en caso que sea necesario
 *
*/
void agrandarArreglo(){

}

//EL código de lectura de archivos hay que probarlo por separado en algún otro lado, hay que probar que esa cosa ande si la vamos a agarrar del Proyecto 1
void LeerArchivo(){

}

void main(){

	int Direcciones [65536]; //Arreglo donde guardaremos las direcciones, valor inicial para 10 direcciones, se debe poder agrandar
	int TablaPaginas [256]; //Tabla de Páginas OJO, va de 0 a 255! cuando accedamos hay que restarle 1 al numero con el que se accederá!
	int framePag; //Numero de frame resultante
	uint16_t DirFis; //Dirección física Resultante
	int TLB [16][2]; //TLB
	inicializar(&Direcciones, &TablaPaginas, &TLB); //función para inicializar todos los arreglos
	uint16_t MaskI = 0000000011111111b; //Máscaras que usaré para obtener el Page Number y el Offset de la Dirección Lógica
	uint16_t MaskS = 1111111100000000b; //Recordar que la dirección Lógica es de 16 bits, necesito comparaciones en 16 bits 
										//(Preguntar igual porque el tema es obtener 8 bits, no 16)
	uint8_t PageNum = 0x00;
	uint8_t Offset = 0x00;
	FILE *Direcc; //Archivo de entrada del proyecto
	Direcc = fopen("memoria.txt");
	LeerArchivo(Direcc, &Direcciones); //Paso el arreglo por puntero (?)
	//Asumo que el archivo se leyó y tengo todas las direcciones en el arreglo "Direcciones" 
	int i;
	uint16_t num; //variable que mantendrá el valor original en 16 bits
	int size = NELEMS(Direcciones); //MACRO para obtener el tamaño final del arreglo
	for(i = 0; i < size; i++){ //Para cada dirección lógica en el arreglo
		num = DecimalABinario(Direcciones[i]); //Obtengo el número de 16 bits en binario 
		//Aquí debo separar en Page Number y Offset, rever esto porque creo que estoy rompiendo el numero original al usar la Máscara
		//Aplico la máscara, dejando los bits que me interesan en la parte superior y luego corto el numero a 8 bits 
		uint16_t numCopy = num;
		PageNum = (numCopy & MaskS) / 100000000; 
		Offset = ((numCopy & MaskI) << 8 ) / 100000000;  //Uso la máscara, como me quedo con los 8 LSB, los muevo para arriba y corto
		framePag = BusquedaTabla(PageNum, &TablaPaginas); //Ver esto del pasaje
		DirFis = concatenar(framePag,Offset);
		printf("Direccion Logica = %d, Direccion Fisica = %d ", num,DirFis); //Hecha la traducción, imprimo, preguntar si es correcto

	}

	return 0;

}