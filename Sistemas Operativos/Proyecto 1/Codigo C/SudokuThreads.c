#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;

/* Estoy usando este tutorial para pthreads:
	https://computing.llnl.gov/tutorials/pthreads/
*/

/*número máximo de Threads que estarán presentes
	1 Thread para todas las filas
	1 Thread para todas las columnas
	9 Threads, 1 para cada Cuadrante
*/

#define NUM_THREADS 11 

/*Estructura para pasar los Argumentos necesarios para verificar una parte del Sudoku
	le paso tanto Fila y Columna Inicial como Fila y Columna Final ya que manejaré 
		a todos los Threads de forma anónima
*/
struct datos_thread{
	int thread_id; //esto lo copié de otro lado, pero dudo de su utilidad, preguntar
	int pos; //posición del arreglo check que se modificará
	int FilaI; //Fila donde empiezo a verificar
	int ColumnaI; //Columna donde empiezo a verificar
	int FilaF; //Fila donde debo terminar (Asumiendo jugada válida)
	int ColumnaF; //Columna donde debo terminar  (Asumiendo jugada válida)
}

typedef enum { false, true } bool; //Definición de un tipo bool


/*Función genérica para todos los threads para verificar una parte del Sudoku
	la Estructura "misDatos" es el cjto de Datos que posee el Thread con el cual
		Se debe manejar para verificar, esta función es independiente de la fila y Columna

 Ejemplo, el Thread que verifica las filas tendrá los siguientes datos en datos_thread:
 	thread_id = ID del thread creado, esto va por cuenta del SO
 	pos = 0;
 	FilaI = 0;
 	FilaF = 8;
	ColumnaI = 0;
	ColumnaF = 8;
*/
void *VerificarParte(void *threadarg){
	
	bool nums[11]; //arreglo para comprobar existencia de los números, lo hago de 11 para hacerlo 1-Based
	struct datos_thread *misDatos;
	misDatos = (struct thread_data *) threadarg; //obtengo la estructura de argumentos que posee el Thread

	for(int F = misDatos-> FilaI; F < misDatos->FilaF && check[pos]; F++){
		for(int C = misDatos-> ColumnaI; C < misDatos->ColumnaF && check[pos]; C++){
			//Verificar parte que me corresponde del Sudoku
			if(nums[C] != false){
				check[pos] = false; //si hay números repetidos mientras leo, corto
			}
			else{
				nums[C] = true; //si el valor es nuevo, subo el flag de que encontré ese número
			}

		}
	}
	for(int i = 1; i <= 11 && check[pos]; i++){
		if(nums[i] != true){ //Si falta un número, corto, la jugada es invalida
			check[pos] = false;
		}
	}
	pthread_exit(NULL);	
}

/*
	Función para modularizar, esta se encarga de leer el archivo
	COMPLETAR, hay que hacer chequeos adicionales y ver si realmente estaria leyendo y asignando el valor
*/
void Lectura(FILE *Sud, char[][9] gril){

	int F = 0;
	int C = 0;
	while(!feof(Sud) && F < 9){
		while(C < 9){
			char num = fgetc(SudokuR); //obtengo el caracter
			if(num != EOF && num != ',' && num != EOL){
				GrillaSudoku[F][C] = num; //si lo leido no es EOF o "," (la grilla esta separada por comas), lo añado a la matriz
				C++;
			} //buscar cuanto era EOF en Linux
		}
		F++;
		C = 0;
	}
}

//Variable Global para obtener los Argumentos para cada Thread
struct datos_thread thread_Data[NUM_THREADS];
//Arreglo de booleanos global para los Threads
bool Check[10];

//Hay que llenar el arreglo de thread_Data

int main(){

	//Variable para manejar el archivo
	FILE *SudokuR;
	//Matriz de 9x9 donde se guardará el sudoku
	char[9][9] GrillaSudoku;
	int rc;
	

	pthread_t Hilos[NUM_THREADS];
	//Cargar thread_Data en algún lado

	//Abro el archivo, para lectura (por el enunciado, debe llamarse "sudoku.txt").
	SudokuR = fopen("sudoku.txt", "r");

	if(!SudokuR){
		//Ocurrió un error al abrir el archivo, reportar dicho error, por ahora solo devuelvo 1, mejorar
		fprintf(stderr, "Ocurrió un error al Abrir el archivo del Sudoku");
		return 1;
	}
	else{
		Lectura(&SudokuR,GrillaSudoku); 
	}


	for(int i = 0; i < NUM_THREADS; i++){
		Check[i] = true; //empiezo inicializando el arreglo, asumiendo que la jugada es válida
	}

	for(int j = 0; j < NUM_THREADS; j++){
		rc = pthread_create(&Hilos[j], NULL, VerificarParte, (void *) &thread_Data[j]);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}


	//Última cosa que el Main debe hacer, esto es para evitar Hilos Zombies
	pthread_exit(NULL);

}