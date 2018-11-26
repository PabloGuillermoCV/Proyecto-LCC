#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/*número máximo de Threads que estarán presentes
	9 Thread para todas las filas
	9 Thread para todas las columnas
	9 Threads, 1 para cada Cuadrante
*/

#define NUM_THREADS 27

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
bool Check[27];
char Grilla[9][9];

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

	int nums[10] = {0,0,0,0,0,0,0,0,0,0}; //arreglo para comprobar existencia de los números, lo hago de 10 para hacerlo 1-Based
	struct datos_thread *misDatos;
	misDatos = (struct datos_thread *) threadarg; //obtengo la estructura de argumentos que posee el Thread

    int F;
	for(F = misDatos->FilaI; F <= misDatos->FilaF && Check[misDatos->pos]; F++){
        int C;
		for(C = misDatos-> ColumnaI; C <= misDatos->ColumnaF && Check[misDatos->pos]; C++){
			//Verificar parte que me corresponde del Sudoku
			int num = Grilla[F][C] - '0';
			if(num < 1 || num > 9) {
                printf("Valor ilegal en la fila y columna %d, %d",F,C);
				Check[misDatos->pos] = false; //si el valor leido es ilegal(esto no deberia pasar, indica error de lectura, termino).
			}
			if(nums[num] == 1){
                printf("Valor Repetido en la fila y columna %d, %d\n",F,C);
				Check[misDatos->pos] = false; //si hay números repetidos mientras leo, corto
			}
			else
				nums[num] = 1; //Si todo va bien, agrego el numero a los numeros encontrados
		}
	}
	int i;
	for(i = 1; i < 10 && Check[misDatos->pos]; i++){
		if(!nums[i]){ //Si falta un número, corto, la jugada es invalida
            printf("Faltaba el numero %d en la Fila y Columna %d, %d\n",i,misDatos->FilaI,misDatos->ColumnaI);
			Check[misDatos->pos] = false;
		}
	}
	pthread_exit(NULL);
}

void Lectura(char GrillaSudoku[][9]){

	int F = 0;
	int C = 0;
	bool corte = false;
	char num;
	FILE *fp;
	fp = fopen("sudoku.txt","r");
	if(fp == NULL)
        printf("error al abrir el archivo\n");
    else{
        while(!feof(fp) && F < 9 && !corte){
            printf("Fila Nro: %d = ",F);
            while(C < 9 && !corte){
                num = fgetc(fp); //obtengo el caracter
                if((int)num != 44 && (int)num < 48 && (int)num > 57)
                    corte = true;
                else{
                    if(num != ',' && num != '\n' && num != ' '){
                        GrillaSudoku[F][C] = num; //si lo leido no es EOL o "," (la grilla esta separada por comas), lo añado a la matriz
                        C++;
                        printf("%c ",num);
                    }
                }
            }
            printf("\n");
            F++;
            C = 0; //Al terminar de leer una fila, paso a la siguiente y reseteo la Columna
        }
	}
	if(corte) //Lei algo ilegal que no es una ","
		printf("Error al procesar la entrada, %c NO es una entrada valida para un sudoku",num);
}

//Hay que llenar el arreglo de thread_Data
void Cargar(){
	int I;
	for(I = 0; I < 9; I++){ //inicializo Verificadores de Filas
		thread_Data[I].thread_id = I;
		thread_Data[I].pos = I;
		thread_Data[I].FilaI = I;
		thread_Data[I].ColumnaI = 0;
		thread_Data[I].FilaF = I;
		thread_Data[I].ColumnaF = 8;
	}

	for(I = 9; I < 18; I++){ //Inicializo verificadores de columnas
		thread_Data[I].thread_id = I;
		thread_Data[I].pos = I;
		thread_Data[I].FilaI = 0;
		thread_Data[I].ColumnaI = I - 9;
		thread_Data[I].FilaF = 8;
		thread_Data[I].ColumnaF = I - 9;
	}

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
	I = 18;
	while(I < 27){ //inicializo verificadores de cuadrantes
        int f;
		for(f = 0; f < 3; f++){
            int c;
			for(c = 0; c < 3; c++){
				thread_Data[I].thread_id = I;
				thread_Data[I].pos = I;
				thread_Data[I].FilaI = fi[f];
				thread_Data[I].ColumnaI = ci[c];
				thread_Data[I].FilaF = ff[f];
				thread_Data[I].ColumnaF = cf[c];
				I++;

			}
		}
	}
}

int main(){


	int rc;



	pthread_t Hilos[NUM_THREADS];
	//Cargar thread_Data en algún lado
	Cargar();

	Lectura(Grilla);


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
	void *status;
	for(j = 0; j < NUM_THREADS; j++){
        if(pthread_join(Hilos[j],&status) != 0){
            perror("error al esperar a un thread\n");

        }
	}


	int n;
	bool TodoVerdadero = true;
	for (n = 0; n < 10 && TodoVerdadero; n++) {
		TodoVerdadero = Check[n];
	}
	if (TodoVerdadero) {
		printf("La jugada de Sudoku era Valida\n");
	}
	else {
		printf("La jugada de Sudoku NO era Valida\n");
	}

	//Última cosa que el Main debe hacer, esto es para evitar Hilos Zombies
	pthread_exit(NULL);

}
