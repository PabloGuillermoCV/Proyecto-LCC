#define __STDC_FORMAT_MACROS //Defino las Macros de Impresion para enteros especiales
#include<stdlib.h>
#include<math.h>
#include <bool.h>
#include<inttypes.h>
#include<stdint.h>

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

	HAY UN TEMA CON INT, PUEDE QUE NO ME ALCANCE PARA REPRESENTAR LOS NUMEROS EN BITS AL HACER CASTEOS, DEPENDE DEL COMPILADOR
	La alternativa a ^ seria usar long long int
*/

/*
 * Dado un numero en decimal, pasarlo a binario, uso método de la División
*/
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

uint8_t DecimalABinario8Bits(int n){
	uint8_t ret = 0x00; //000000000000000 en binario usando representación en Hexa
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

	while(n > 0){
		rem = n % 10;
		decimal = decimal + rem * base;
		n = n / 10;
		base = base * 2;
	}

	return decimal;
}

int BinarioADecimal8bits(uint8_t n){
	int rem,decimal,base = 1;

	while(n > 0){
		rem = n % 10;
		decimal = decimal + rem * base;
		n = n / 10;
		base = base * 2;
	}

	return decimal; 
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
* función para separar un entero de 16 bits en dos enteros de 8 bits
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
uint8_t BusquedaTabla(uint8_t PN, uint8_t TP[]){
	int Nro = BinarioADecimal8bits(PN);
	return TP[Nro + 1];
}

/*
 * Dado un número de Página, lo intento buscar en el TLB, en caso de no encontrarlo, debo bajar a la Tabla de Páginas
 * Lo que devuelvo es la POSICIÓN en donde encontré el numero de página o -1 en caso de TLB Miss
*/
int BusquedaTLB(uint8_t PN, uint8_t TLB [][2]){
	int C;
	bool ret = true;
	//Buscar valor aquí
	for(C = 0; C <= 15 && ret; C++){
		ret = TLB[C][0] == PN;
	}
	if(ret == 0)
		C = TLB_MISS;

	return C;
}

/*
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Mi idea es usar un algoritmo FIFO para reemplazar, preguntar
	Asumo que la página más vieja es aquella que esta al final del arreglo!
*/
void Reemplazar(uint8_t FP, uint8_t PN, uint8_t TLB[][2]){
	int i;

	//Lo que debo hacer es correr TODAS las posiciones un lugar hacia abajo, dejando el primer espacio libre
	//de tal forma que la primer componente de la matriz quede libre para el nuevo valor
	TLB[15][0] = 0;
	TLB[15][1] = 0; //Borro la ultima componente (la más vieja) del TLB, dejando un espacio libre
	for(i = 15; i >= 0; i--){
		TLB[i][0] = TLB[i-1][0]; //Muevo los Page Number existentes hacia abajo
		TLB[i][1] = TLB [i -1][1]; //Muevo los FP existentes hacia abajo
	}

	//Hecho el reemplazo, agrego las nuevas componentes
	TLB[0][0] = PN;
	TLB[0][1] = FP;

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
    	exit(1);

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


/*
 * Función para determinar si estoy intentando insertar un valor repetido en la Tabla de Paginas 
*/
bool Repetido(uint8_t Pages[], uint8_t num){
	bool ret = true;
	int i;
	for(i = 0; i < NELEMS(Pages) && ret; i++){
		ret = Pages[i] == num;
	}

	return ret;
}

/*
 * funcion para inicializar los arreglos de direcciones input y Tabla de Paginas
*/
void inicializar(int DirIni[], uint8_t PageT[]){
	int i;
	for(i = 0; i < NELEMS(DirIni); i++){
		DirIni[i] = -1;
	}
	for(i = 0; i < NELEMS(PageT); i++){
		int r = rand() % 255; //numero random para llenar la tabla de páginas
		uint8_t r8 = DecimalABinario8bits(r);
		if(!Repetido(PageT, r8)) //Si el numero generado NO esta repetido dentro del arreglo, lo pongo como numero de frame
			PageT[i] = r8;
	}
}


void main(){

	int Direcciones [65536]; //Arreglo donde guardaremos las direcciones
	uint8_t TablaPaginas [256]; //Tabla de Páginas OJO, va de 0 a 255! cuando accedamos hay que restarle 1 al numero con el que se accederá!
	uint8_t framePag; //Numero de frame resultante
	uint16_t DirFis; //Dirección física Resultante
	uint8_t direcc [2]; //arreglo para los cortes de 8 bits
	uint8_t TLB [16][2]; //TLB
	inicializar(Direcciones, TablaPaginas); //función para inicializar todos los arreglos (componentes de Direcciones se inicializa en -1)
	uint8_t PageNum = 0x00;
	uint8_t Offset = 0x00;
	FILE *Direcc; //Archivo de entrada del proyecto
	Direcc = "memoria.txt";
	LeerArchivo(Direcc, Direcciones); 
	//Asumo que el archivo se leyó y tengo todas las direcciones en el arreglo "Direcciones" 
	int i;
	bool corte = false;
	uint16_t num; //variable que mantendrá el valor original en 16 bits
	for(i = 0; i < size && !corte; i++){ //Para cada dirección lógica en el arreglo
		if(Direcciones[i] != -1){ //Hago este chequeo por las dudas
			int OG = Direcciones[i];
			num = DecimalABinario(Direcciones[i]); //Obtengo el número de 16 bits en binario  
			uint16_t numCopy = num;
			direcc = convertFrom16To8(numCopy);
			PageNum = direcc[0];
			Offset = direcc[1]; 
			framePag = BusquedaTabla(PageNum, TablaPaginas);
			DirFis = convertFrom8To16(framePag,Offset);
			printf("Direccion Logica = %d, Direccion Fisica asociada = %"PRIu16" \n", OG,BinarioADecimal(DirFis)); //Hecha la traducción, imprimo, preguntar si es correcto
		}
		corte = Direcciones[i+1] == -1; //Como NO es probable que me llenen el arreglo, hago un "peek" para ver si debo seguir traduciendo

	}

	printf("Se ha terminado de traducir las direcciones logicas presentadas en el archivo\n");
	return 0;

}