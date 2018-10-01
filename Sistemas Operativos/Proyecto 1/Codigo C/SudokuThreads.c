#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

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
};

//Variable Global para obtener los Argumentos para cada Thread
struct datos_thread thread_Data[NUM_THREADS];
//Arreglo de booleanos global para los Threads
bool Check[10];

//Matriz de 9x9 donde se guardará el sudoku
char GrillaSudoku [9][9];

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

	bool nums[10]; //arreglo para comprobar existencia de los números, lo hago de 10 para hacerlo 1-Based
	struct datos_thread *misDatos;
	misDatos = (struct thread_data *) threadarg; //obtengo la estructura de argumentos que posee el Thread

    int F;
	for(F = misDatos->FilaI; F < misDatos->FilaF && Check[misDatos->pos]; F++){
        int C;
		for(C = misDatos-> ColumnaI; C < misDatos->ColumnaF && Check[misDatos->pos]; C++){
			//Verificar parte que me corresponde del Sudoku
			int num = GrillaSudoku[F][C] - '0';
			if(num < 1 || num > 9) {
				Check[misDatos->pos] = false; //si el valor leido es ilegal(esto no deberia pasar, indica error de lectura, termino).
			}
			if(nums[num] != false){
				Check[misDatos->pos] = false; //si hay números repetidos mientras leo, corto
			}
		}
	}
	int i;
	for(i = 1; i < 10 && Check[misDatos->pos]; i++){
		if(nums[i] != true){ //Si falta un número, corto, la jugada es invalida
			Check[misDatos->pos] = false;
		}
	}
	pthread_exit(NULL);
}

/*
	Función para modularizar, esta se encarga de leer el archivo
	COMPLETAR, hay que hacer chequeos adicionales y ver si realmente estaria leyendo y asignando el valor
*/
void Lectura(FILE *SudokuR){

	int F = 0;
	int C = 0;
	while(!feof(SudokuR) && F < 9){
		while(C < 9){
			char num = fgetc(SudokuR); //obtengo el caracter
			int x = num - '0';
			if(num != EOF && num != ',' && num != '\n' && !(x < 1 || x > 9)  ){
				GrillaSudoku[F][C] = num; //si lo leido no es EOF o "," (la grilla esta separada por comas), lo añado a la matriz
				C++;
			} //buscar cuanto era EOF en Linux
		}
		F++;
		C = 0;
	}
}

//Hay que llenar el arreglo de thread_Data
void Cargar(){
	thread_Data[0].thread_id = 0;
	thread_Data[0].pos = 0;
	thread_Data[0].FilaI = 0;
	thread_Data[0].ColumnaI = 8;
	thread_Data[0].FilaF = 0;
	thread_Data[0].ColumnaF = 8;
	
	thread_Data[1].thread_id = 1;
	thread_Data[1].pos = 1;
	thread_Data[1].FilaI = 0;
	thread_Data[1].ColumnaI = 0;
	thread_Data[1].FilaF = 0;
	thread_Data[1].ColumnaF = 8;
	
	int fi [3] = {0, 3, 6};
	int ci [3] = {0, 3, 6};
	int ff [3] = {2, 5, 8};
	int cf [3] = {2, 5, 8};
	/* (A Efectos de comprobar que el triple ciclo de abajo genera los siguientes conjuntos)
	thread_Data[2] = { 2, 2, fi[0], ci[0], ff[0], cf[0]};
	thread_Data[3] = { 3, 3, fi[0], ci[1], fi[0], cf[1]};

	thread_Data[4] = { 4, 4, fi[0], ci[2], fi[0], cf[2]};
	thread_Data[5] = { 5, 5, fi[1], ci[0], fi[1], cf[0]};
	thread_Data[6] = { 6, 6, fi[1], ci[1], fi[1], cf[1]};
	thread_Data[7] = { 7, 7, fi[1], ci[2], fi[1], cf[2]};
	thread_Data[8] = { 8, 8, fi[2], ci[0], fi[2], cf[0]};
	thread_Data[9] = { 9, 9, fi[2], ci[1], fi[2], cf[1]};
	thread_Data[10] = { 10, 10, fi[2], ci[2], fi[2], cf[2]};*/
	int i = 2;
	while(i < NUM_THREADS){
        int f;
		for(f = 0; f < 3; f++){
            int c;
			for(c = 0; c < 3; c++){
				thread_Data[i].thread_id = i;
				thread_Data[i].pos = i;
				thread_Data[i].FilaI = fi[f];
				thread_Data[i].ColumnaI = ci[c];
				thread_Data[i].FilaF = ff[f];
				thread_Data[i].ColumnaF = cf[c];
				i++;
			}
		}
	}
}

int main(){

	//Variable para manejar el archivo
	FILE *SudokuR;
	
	int rc;


	pthread_t Hilos[NUM_THREADS];
	//Cargar thread_Data en algún lado
	Cargar();

	//Abro el archivo, para lectura (por el enunciado, debe llamarse "sudoku.txt").
	SudokuR = fopen("sudoku.txt", "r");

	if(!SudokuR){
		//Ocurrió un error al abrir el archivo, reportar dicho error, por ahora solo devuelvo 1, mejorar
		fprintf(stderr, "Ocurrió un error al Abrir el archivo del Sudoku");
		return 1;
	}
	else{
		Lectura(SudokuR);
	}

    int i;
	for(i = 0; i < NUM_THREADS; i++){
		Check[i] = true; //empiezo inicializando el arreglo, asumiendo que la jugada es válida
	}

    int j;
	for(j = 0; j < NUM_THREADS; j++){
		rc = pthread_create(&Hilos[j], NULL, VerificarParte, (void *) &thread_Data[j]);
		if (rc){ //ocurrió un error al crear el Thread, reportar
        	printf("ERROR AL CREAR HILOS; Código de retorno: %d\n", rc);
        	exit(-1);
       	}
	}
	
	int n;
	bool TodoVerdadero = true;
	for (n = 0; n < 10 && TodoVerdadero; n++) {
		TodoVerdadero = Check[n];
	}
	if (TodoVerdadero == true) {
		printf("La jugada de Sudoku era Valida");
	}
	else {
		printf("La jugada de Sudoku NO era Valida");
	}
	
	//Última cosa que el Main debe hacer, esto es para evitar Hilos Zombies
	pthread_exit(NULL);

}