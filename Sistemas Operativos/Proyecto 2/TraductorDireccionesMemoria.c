#include<stdlib.h>
#include<math.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera

#define TLB_MISS -1

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
		Hecho todo el trámite, termino con al dirección física a devolver 

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

	preguntar que deberia contener la tabla de paginas, si numeros random o algo en particular

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

//estas funciones de conversión entre numeros en bits ESTAN PROBADAS Y ANDAN
/*
 * Función para transformar dos enteros de 8 Bits en un entero de 16 Bits
*/
uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond) {
    uint16_t dataBoth = 0x0000;

    dataBoth = dataFirst;
    dataBoth = dataBoth << 8; //Muevo el primer valor a la parte alta del numero de  16 bits
    dataBoth |= dataSecond; //Hago OR entre mi valor actual y el segundo valor introducido a la función, el OR hará la suma en binario entre los bits
    return dataBoth;
}

/*
* función para separar un entero de 16  bits en dos enteros de 8 bits
* los resultados se devuelven en un arreglo de dos componentes
*/
uint8_t *convertFrom16To8(uint16_t dataAll) {
    static uint8_t arrayData[2] = { 0x00, 0x00 };

    *(arrayData) = (dataAll >> 8) & 0x00FF; //Obtengo la parte alta del numero de 16 bits y la mando a la primer componente del arreglo
    arrayData[1] = dataAll & 0x00FF; //Obtengo la parte baja del numero de 16 bits
    return arrayData;
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
int BusquedaTLB(uint8_t PN, int TLB [][2]){

	int ret = 0;

	//Buscar valor aquí
	for(int C = 0; C <= 15 && ret == 0; C++){

	}
	if(ret == 0)
		ret = TLB_MISS;

	return ret;
}

/*
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Mi idea es usar un algoritmo FIFO para reemplazar, preguntar
*/
void Reemplazar(int FP, uint8_t PN){
	int i = 0;
	

}


//EL código de lectura de archivos hay que probarlo por separado en algún otro lado, hay que probar que esa cosa ande si la vamos a agarrar del Proyecto 1
void LeerArchivo(FILE *file, int Dirs[]){
	char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    const char *errstr = NULL;
    FILE *fp = fopen(file, "r"); //Abro el archivo en modo lectura
    if(fp = NULL)
    	exit(EXIT_FAILURE);

     while ((read = getline(&line, &len, fp)) != -1) {
       //Guardo en line la linea, que en realidad es el numero que debo guardar
     	int num = strtonum(line, 0 , 65536, errstr); //el 0 y 65536 representan los numeros mínimo y máximo que acepto, respectivamente
     	if(errstr != NULL){
     		fprintf(stderr, "Error al intentar obtener las direcciones logicas con mensaje %s", errstr);
     		exit(1);
     	}
     	//Si no salí en este punto, todo salió bien y en num tengo la direccion logica a traducir
     	Dirs[i] = num; 
     	i++;
    }

    fclose(fp); //cierro el archivo
    if (line)
        free(line); //Libero la memoria reservada
 
}

void main(){

	int Direcciones [65536]; //Arreglo donde guardaremos las direcciones
	int TablaPaginas [256]; //Tabla de Páginas OJO, va de 0 a 255! cuando accedamos hay que restarle 1 al numero con el que se accederá!
	int framePag; //Numero de frame resultante
	uint16_t DirFis; //Dirección física Resultante
	uint8_t direcc [2]; //arreglo para los cortes de 8 bits
	int TLB [16][2]; //TLB
	inicializar(Direcciones, TablaPaginas, TLB); //función para inicializar todos los arreglos
	uint8_t PageNum = 0x00;
	uint8_t Offset = 0x00;
	FILE *Direcc; //Archivo de entrada del proyecto
	Direcc = "memoria.txt";
	LeerArchivo(Direcc, Direcciones); 
	//Asumo que el archivo se leyó y tengo todas las direcciones en el arreglo "Direcciones" 
	int i;
	uint16_t num; //variable que mantendrá el valor original en 16 bits
	for(i = 0; i < size; i++){ //Para cada dirección lógica en el arreglo
		int OG = Direcciones[i];
		num = DecimalABinario(Direcciones[i]); //Obtengo el número de 16 bits en binario 
		//Aquí debo separar en Page Number y Offset, rever esto porque creo que estoy rompiendo el numero original al usar la Máscara
		//Aplico la máscara, dejando los bits que me interesan en la parte superior y luego corto el numero a 8 bits 
		uint16_t numCopy = num;
		direcc = convertFrom16To8(numCopy);
		PageNum = direcc[0];
		Offset = direcc[1];
		framePag = BusquedaTabla(PageNum, TablaPaginas); //Ver esto del pasaje
		DirFis = convertFrom8To16(framePag,Offset);
		printf("Direccion Logica = %d, Direccion Fisica = %d ", OG,BinarioADecimal(DirFis)); //Hecha la traducción, imprimo, preguntar si es correcto

	}

	return 0;

}