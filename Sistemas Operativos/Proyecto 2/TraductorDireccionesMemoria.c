#define __STDC_FORMAT_MACROS //Defino las Macros de Impresion para enteros especiales
#include<stdlib.h>
#include<math.h>
#include<inttypes.h>
#include<stdint.h>
#include<stdio.h>
#include<time.h>
#include<stdbool.h>

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera

#define TLB_MISS -1 //Codigo especial para los TLB Miss que se puedan provocar

/*PASOS A SEGUIR:

	0) Leer el archivo y obtener todas las direcciones lógicas a traducir
	1) Obtener la Dirección Lógica
		Lo que debo hacer es, dado el numero entero, Pasarlo a Base 2
		y de ahí usar Enmascarado de Bits para separar el numero de 16 bits resultante en dos secuencias de 8 bits
		que serán el Numero de Página y el Offset
	2) Acceder con el Número de TLB
		En primera Instancia, una vez que se el numero de página, accedo al TLB (el cual es una matriz de 16x3, donde La tercer columna del TLB los uso como si fuesen "Dirty Bits", al inciar, los pongo en 0 y con estos puedo determinar si una entrada del TLB
				es o inicial y puede ser reemplazada o no) y busco el Frame correspondiente, en caso de NO encontrar el numero de página en el TLB (Osea, se produjo un TLB_MISS),
				debo bajar a la Tabla de Páginas, encontrar el numero de frame y el par (NroPágina,NroFrame) agregarlo al TLB, si NO lo puedo agregar al TLB porque el mismo NO
				dispone de espacio, debo reemplazar una entrada en el mismo, esto se lleva a cabo por medio de un algoritmo de reemplazo FIFO, donde la entrada más vieja coincide con el último
				par (NroPágina,NroFrame) de la Matriz
	3) Obtener la Dirección Física
		Obtenido el Nro de Frame, lo concateno con el Offset y devuelvo el resultado de la concatenación EN DECIMAL, el cual es la dirección Física final

	Sobre uint_x:
		0x.. -> el número lo represento en Hexadecimal
		0b.. -> el número lo represento en Binario
		Sin literal despues del primer cero lo estoy trabajando en Decimal
		También puedo representar el numero completo y poner el literal de representación al final
		1111b es un número binario de 4 bits, por ejemplo

	Sobre Enmascarado:
		Yo al enmascarar, me quedo con los bits que me "Importan" al aplicar una operación AND entre el
		Numero original en binario y un número binario arbitrario cuyas posiciones en 1 denotan QUE posiciones de bits dejaré como están

		asumamos que el numero es 101011010111111 y la máscara es 1111111100000000
		al hacer 101011010111111 & 1111111100000000 obtengo 1010110100000000

	Sobre Shifting:
		la operación >> o << denota un shifting de los bits del número a derecha o a izquierda respectivamente 
		se permite usar potencias de dos para hacer shifting de varias posiciones de una.
		LOS BITS QUE SE CAEN DEL NÚMERO SE PIERDEN
		si mi numero es 010110, hacer 010110 >> resulta en 001011
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

	return ret;

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

	return ret;

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


int BinarioADecimal8bits(uint8_t n){
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


//estas funciones de conversión entre numeros en bits ESTAN PROBADAS Y ANDAN
/*
 * Función para transformar dos enteros de 8 Bits en un entero de 16 Bits
*/
uint16_t pasarDe8A16(uint8_t dataFirst, uint8_t dataSecond) {
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
uint8_t *pasarDe16A8(uint16_t dataAll) {
    static uint8_t arrayData[2] = {0x00,0x00};

    arrayData[0] = (dataAll >> 8) & 0x00FF; //Obtengo la parte alta del numero de 16 bits y la mando a la primer componente del arreglo
    arrayData[1] = dataAll & 0x00FF; //Obtengo la parte baja del numero de 16 bits
    return arrayData;
}


/*
 * Dado un numero de página, buscar el numero de frame correspondiente en la Tabla de Páginas
*/
uint8_t BusquedaTabla(uint8_t PN, uint8_t TP[]){
	int Nro = BinarioADecimal8bits(PN);
	return TP[Nro];
}

/*
 * Dado un número de Página, lo intento buscar en el TLB, en caso de no encontrarlo, debo bajar a la Tabla de Páginas
 * Lo que devuelvo es la POSICIÓN en donde encontré el numero de página o -1 en caso de TLB Miss
*/
int BusquedaTLB(uint8_t PN, uint8_t TLB [][3]){
	int C;
	int ret = 0;
	//Buscar valor aquí
	for(C = 0; C <= 15 && ret == 0; C++){
		if(TLB[C][0] == PN)
			ret = 1;
	}
	if(ret == 0)
		C = TLB_MISS;

	return C;
}

/*
	Método que despues se encargará de hacer un reemplazo cuando el TLB este lleno
	Usa un ALgoritmo de reemplazo FIFO donde la página más vieja es aquella que esta al final del arreglo
*/
void Reemplazar(uint8_t FP, uint8_t PN, uint8_t TLB[][3]){
	int i;

	//Lo que debo hacer es correr TODAS las posiciones un lugar hacia abajo, dejando el primer espacio libre
	//de tal forma que la primer componente de la matriz quede libre para el nuevo valor
	TLB[15][0] = 0;
	TLB[15][1] = 0; //Borro la ultima componente (la más vieja) del TLB, dejando un espacio libre
	for(i = 15; i >= 0; i--){
		TLB[i][0] = TLB[i-1][0]; //Muevo los Page Number existentes hacia abajo
		TLB[i][1] = TLB [i -1][1]; //Muevo los FP existentes hacia abajo
		TLB[i][2] = TLB [i-1][2]; //Muevo los Dirty Bits existentes un lugar para abajo
	}

	//Hecho el reemplazo, agrego las nuevas componentes
	TLB[0][0] = PN;
	TLB[0][1] = FP;
	TLB[0][2] = 1; //marco que es una nueva entrada que NO puede ser reemplazada

}


//EL código de lectura de archivos hay que probarlo por separado en algún otro lado, hay que probar que esa cosa ande si la vamos a agarrar del Proyecto 1
void LeerArchivo(int Dirs[]){
    int i = 0;
    int num = 0;
    FILE *fp = fopen("memoria.txt", "r"); //Abro el archivo en modo lectura
    if(fp == NULL){
        printf("Error al abrir el archivo para lectura!");
        exit(1);
    }
    else{
        while(!feof(fp)){
            fscanf(fp,"%d", &num);
            Dirs[i] = num;
            i++;
        }
    }
    fclose(fp); //cierro el archivo
}


/*
 * Función para determinar si estoy intentando insertar un valor repetido en la Tabla de Paginas
*/
bool Repetido(uint8_t Pages[], uint8_t num){
	bool ret = false;
	int i;
	for(i = 0; i < NELEMS(Pages) && !ret; i++){
		ret = Pages[i] != 0x00 && Pages[i] == num;  //0 es un caso especial ya que PUEDE que alguna página tenga Nro de Frame = 0

	}

	return ret;
}

/*
 * funcion para inicializar los arreglos de direcciones input y Tabla de Paginas
*/
void inicializar(int DirIni[], uint8_t PageT[], uint8_t TLB[][3]){
	int i;
	time_t t;
	srand((unsigned) time(&t)); //Seteo el "seed" para la aleatoriedad de rand() usando la hora actual del equipo
	//Inicializo arreglo de direcciones logicas
	for(i = 0; i <= 65536; i++){
		DirIni[i] = -1; //Leno el arreglo de direcciones con "-1" para luego poder hacer Peek en caso que NO me llenen el arreglo enteramente
	}
	//Inicializo Tabla de Páginas
	for(i = 0; i <= 255; i++){
		int r = rand() % 255; //numero random para llenar la tabla de páginas
		uint8_t r8 = DecimalABinario8Bits(r);
		if(!Repetido(PageT, r8)) //Si el numero generado NO esta repetido dentro del arreglo, lo pongo como numero de frame
			PageT[i] = r8;
		else
			i--; //Fuerzo al ciclo a repetir la acción para que genere un nuevo numero en esa posición
	}
	//INicializo TLB
	for(i = 0; i < 16; i++){
		TLB[i][0] = 0;
		TLB[i][1] = 1;
		TLB[i][2] = 0; //"Dirty Bit"
	}

}

/*
 * Función que intenta agregar una nueva entrada al TLB cuando se produjo un TLB_MISS
*/
void AgregarTLB(uint8_t PN, uint8_t FN, uint8_t TLB[][3]){
	int i;
	for(i = 0; i < 16; i++){
		if(TLB[i][0] == 0 & TLB[i][1] == 1 & TLB[i][2] == 0){ //Si encuentro una "Entrada Inicial"
			TLB[i][0] = PN;
			TLB[i][1] = FN;
			TLB[i][2] = 1; //Marco que esta entrada fue modificada
		}
		else{
			Reemplazar(FN,PN,TLB); //Si no, debo reemplazar una entrada para agregar la nueva, delego en el algoritmo de reemplazo
		}
	}

}

int main(){

	int Direcciones [65536]; //Arreglo donde guardaremos las direcciones
	uint8_t TablaPaginas [256]; //Tabla de Páginas
	uint8_t framePag; //Numero de frame resultante
	uint16_t DirFis; //Dirección física Resultante
	uint8_t *Cortes8Bit; //arreglo para los cortes de 8 bits
	uint8_t TLB [16][3]; //TLB
	inicializar(Direcciones, TablaPaginas, TLB); //función para inicializar los arreglos
	uint8_t PageNum = 0x00;
	uint8_t Offset = 0x00;
	LeerArchivo(Direcciones);
	//Asumo que el archivo se leyó y tengo todas las direcciones en el arreglo "Direcciones"
	int i;
	int corte = 0;
	uint16_t num; //variable que mantendrá el valor original en 16 bits
	for(i = 0; i < 65537 && corte == 0; i++){ //Para cada dirección lógica en el arreglo
		if(Direcciones[i] != -1){ //Hago este chequeo por las dudas
			int OG = Direcciones[i];
			num = DecimalABinario(Direcciones[i]); //Obtengo el número de 16 bits en binario
			uint16_t numCopy = num;
			Cortes8Bit= pasarDe16A8(numCopy);
			PageNum = Cortes8Bit[0];
			Offset = Cortes8Bit[1];
			int BT = BusquedaTLB(PageNum, TLB); //En primera instancia, busco si el Page Number esta en el TLB
			if(BT == TLB_MISS){ //Si NO se encontró el numero de Frame en el TLB, debo bajar a la Tabla de Páginas y luego agregar la entrada al TLB
				framePag = BusquedaTabla(PageNum, TablaPaginas);
				AgregarTLB(PageNum, framePag, TLB);
			}
			else{ //Si lo encontré en el TLB, solo obtengo el numero de frame y devuelvo los datos, en BT tengo la Fila donde encontré la entrada
				framePag = TLB[BT][1];
			}
			DirFis = pasarDe8A16(framePag,Offset); //Obtengo la Dirección Física final
			printf("Direccion Logica = %d, Direccion Fisica asociada = %d \n", OG,BinarioADecimal(DirFis)); //Hecha la traducción, imprimo, preguntar si es correcto
		}
		if(Direcciones[i+1] == -1) //Como NO es probable que me llenen el arreglo, hago un "peek" para ver si debo seguir traduciendo
			corte = -1;
	}

	printf("Se ha terminado de traducir las direcciones logicas presentadas en el archivo\n");
	return 0;

}
