#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

typedef enum { false, true } bool; //Definición de un tipo bool

/* Como seria la idea general para resolver el tester de Sudoku?
   1) generar la matriz -> 1 Proceso separado del Padre
   		1.1)sabemos que la Matriz es de 9x9, asi que un arreglo bi-dimensional cuadrado de caracteres bastará
		1.2)Es necesario abrir el archivo de entrada (fopen()), leer caracter por caracter
			(al usar caracteres, estamos usando ASCII, ver esto)
			1.2.1) mientras se lee el archivo (mientras NOT feof() -> fgetc(). Leer caracter a caracter), se carga la matriz
			1.2.2) una vez leido el archivo y cargada la matriz, se cierra el archivo (fclose())
	2)Empezar a bifurcar en serio
		2.1)la idea seria usar 3 procesos distintos y hacer que el Padre espere a los 3 hijos
			2.1.1) Proceso que verifique las Filas del Sudoku
				Este verificará fila por fila que en esa fila esten los números del 1 al 9 (SIN REPETICIÓN)
			2.1.2) Proceso que verifique las Columnas del Sudoku
				Este verificará Columna por Columna que en esa columna esten los números del 1 al 9 (SIN REPETICIÓN)
			2.1.3) Proceso que verifique los cuadrantes 3x3 del Sudoku
				Este verificará por Cuadrante (hay 9 cuadrantes, empezando en las posiciones (0,0), (0,3)  (0,6)
				(3,0), (3,3), (3,6) , (6,0) , (6,3) , (6,6))
		2.2) Estos Procesos deben reportar si su tarea falló en caso de que la jugada sea inválida, al primero que lo hace, se
			 debe cortar TODO, el padre debe matar a los hijos restantes (se podria hacer con una variable global, pero lo dudo)
		2.3) si NINGUNO falla y TODOS terminan, entonces la jugada de Sudoku era válida, entonces, el Padre reporta que la jugada era válida
	3) Consideraciones
		3.1) Los Procesos pueden fallar por si solos en cualquier momento, hay que ver las excepciones que pueden tirar y sus códigos
		3.2) Ver lo de la Variable Global para controlar que todo este en órden
		3.3) Una vez hecho esto con procesos, pasarlo a Threads es tan fácil como reemplazar los Procesos por Threads
		3.4) Verificar si es mejor usar char[][] o int[][], si es mejor usar int, hay que tener en cuenta que hay que traducir
			 el ASCII del caracter al número INT
		3.5) Preguntar sobre como reportar errores (códigos de error? Strings?)
		3.6) Preguntar sobre input de funciones por Punteros (no recuerdo si asi como lo hice esta bien o va con "&" al llamar)
		3.7) recordar que las funciones de archivos devuelven un int, ver que hacer con esto
		3.8) parece que exit() no puede hacer más que devolver 0 (terminación normal) o 1 (terminación anormal), existen
			 un par de funciones llamadas <int atexit (void (* function) (void))> y <int on_exit(void (*function)(int , void *), void *arg)>
			 que permiten ejecutar funciones cuando se llama a exit(), preguntar sobre su uso y como hacer el pasaje
			 (ya que requiere pasajes de función por punteros, lo que hicimos en Orga para el Primer Proyecto)
		3.9) El Chequeo para saber donde estoy al hacer fork() se repite siempre, preguntar si no se puede modularizar en un función
		3.10) Mientras se está chequeando las Filas y Columnas, siempre avanzar para adelante
			3.10.1) Ver como chequear que esten todos lo números (se me ocurre un array de booleans/int 1-based, entonces, si hay un 1
					en las 9 posiciones del arrgelo, la fila/columna/cuadrante es correcta, si hay algún número en 0 o hay un número
					con más de una aparición, la fila/columna/cuadrante es incorrecta)
		3.11) Ver como era el pasaje de Arreglos a funciones

*/

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


/*Método que, dependiendo del número pasado, discrimina la tarea a asignarle al proceso entrante
	si es el primer Proceso, le asigno que tiene que verificar las filas, si es el segundo, las columnas, sino,
	es el tercero y le asigno revisar los cuadrantes con sus respectivas funciones especiales dentro de un loop while
*/
bool HacerTarea(int ProcNum){

	int rec,Y = 0;
	bool check = true;

	switch(ProcNum){

		case 0:
			while(check && rec < 9){
				check = VerificarFila(GrillaSudoku, rec);
				rec++;
			}
			break;
		case 1:
			while(check && rec < 9){
				check = VerificarColumna(GrillaSudoku, rec);
				rec++;
			}
			break;
		case 2:
			while(check && rec <= 6 && Y <= 6){
				check = VerificarCuadrante(GrillaSudoku, rec, Y);
				rec = rec + 3;
				if(rec == 6 && Y != 6){
					rec = 0;
					Y = Y + 3;
				}
			}
			break;
	}
	return check;
}


/* Función que verificará un Cuadrante del Sudoku
*	gril, grilla del Sudoku
*	F, Fila de Inicio del cuadrante
*	al retornar, devuelve Verdadero (la fila es correcta) o Falso (la Fila NO es válida, aquí se debe terminar todo)
*/
bool VerificarFila(char[][9] gril, int F){
    bool Lista [9];
    int I;
    for (I = 0; I < 9; I++) {
        int X = gril[F][I] - '0';
        if (X < 1 || X > 9) {
            //Tengo un valor no posible
            return false;
        }
        if (Lista[X] == true) {
            //Me encuentro con un valor repetido
            return false;
        }
        Lista[X] = true;
    }
    for (I = 0; I < 9; I++) {
        //Verifico que esten todos los numeros
        int X = I + 1;
        if (Lista[X] == false) {
            //Faltaba un valor en la lista
            return false;
        }
    }
	return true;
}

/* Función que verificará una Columna del Sudoku
*	gril, grilla del Sudoku
*	C, Columna de Inicio del Cuadrante
*	al retornar, devuelve Verdadero (La Columna es correcta) o Falso (La Columna NO es válida, aquí se debe terminar todo)
*/
bool VerificarColumna(char[][9] gril, int C){
    bool Lista [9];
    int I;
    for (I = 0; I < 9; I++) {
        int X = gril[I][C] - '0';
        if (X < 1 || X > 9) {
            //Tengo un valor no posible
            return false;
        }
        if (Lista[X] == true) {
            //Me encuentro con un valor repetido
            return false;
        }
        Lista[X] = true;
    }
    for (I = 0; I < 9; I++) {
        //Verifico que esten todos los numeros
        int X = I + 1;
        if (Lista[X] == false) {
            //Faltaba un valor en la lista
            return false;
        }
    }
	return true;
}

/* Función que verificará un Cuadrante del Sudoku
*	gril, grilla del Sudoku
*	X, Fila de Inicio del cuadrante
*	Y, Columna de Inicio del Cuadrante
*	al retornar, devuelve Verdadero (El cuadrante es correcto) o Falso (El Cuadrante NO es válido, aquí se debe terminar todo)
*/
bool VerificarCuadrante(char[][9] gril, int X, int Y){
    bool Lista [9];
    int I;
    int J;
    for (I = X; I < X+3; I++) {
        for (J = Y; J < Y+3; J++) {
            int X = gril[I][J] - '0';
            if (X < 1 || X > 9) {
                //Tengo un valor no posible
                return false;
            }
            if (Lista[X] == true) {
                //Me encuentro con un valor repetido
                return false;
            }
            Lista[X] = true;
        }
    }
    for (I = 0; I < 9; I++) {
        //Verifico que esten todos los numeros
        int X = I + 1;
        if (Lista[X] == false) {
            //Faltaba un valor en la lista
            return false;
        }
    }
    return true;
}


int main(){

	//Variable para manejar el archivo
	FILE *SudokuR;
	FILE *Proc1;
	FILE *Proc2;
	FILE *Proc3;
	//Matriz de 9x9 donde se guardará el sudoku
	char[9][9] GrillaSudoku;
	int processCount = 2;
	bool check1 = true;
	int pid = 0;
	//Abro el archivo, para lectura (por el enunciado, debe llamarse "sudoku.txt").
	SudokuR = fopen("sudoku.txt", "r");

	if(!SudokuR){
		//Ocurrió un error al abrir el archivo, reportar dicho error, por ahora solo devuelvo 1, mejorar
		return 1;
	}
	/*
	 el archivo se abrió correctamente, hago el primer fork() para leer el archivo, mi hijo compartirá
	 el archivo abierto por lo dicho en la teoria Y LA MATRIZ, además, ámbos ejecutan concurrentemente por UNIX
	 El Padre debe esperar a este Hijo
	*/
	else{
		pid = fork();

		if(pid == -1){
			//Hubo un error al crear el hijo, reporto dicha ocurrencia (reportar)
		}
		else{
			if(pid > 0){
				//Estoy en el Padre, en este caso, debo esperar a que mi hijo termine y devuelva
				wait(NULL);
			}
			else{
				//Estoy en el hijo, debo leer el archivo y cargar la matriz, delego en un procedimiento
				//para modularizar
				Lectura(SudokuR, GrillaSudoku);
				//terminado el proceso, yo como hijo debo reportar que terminé
				exit(0);
			}

			//asumiendo que terminé de leer todo correctamente, deberia tener la matriz totalmente cargada
			//ya no necesito más el archivo, puedo cerrarlo y continuar
			fclose(SudokuR);
		}
	}
	/*
		Ciclo FOR que crea todos los procesos necesarios, luego, si estoy en el Hijo,
		Entro a un método especial que le asigna un trabajo a realizar
	*/
	for(int i=0; i<processCount && check1; i++){
    	pid=fork();
    	if(pid == -1){
    		//tirar error, algo salió mal al crear el hijo
    	}
    	else{
    		if(pid != 0){
        		check1 = HacerTarea(i); //Entro al método para saber que hacer yo como Hijo
        		//OJO, ver donde meter el exit() para terminar el hijo
        		exit(); //una vez que yo, POroceso Hijo, termino mi tarea, hago Exit.
        		break;
    		}
    	}

	}
	if (pid == 0){
    	wait(NULL);
	}

	//Chequeo que todo esté bien, esto es, leo los 3 archivos y si todos tienen "true", entonces estaba todo bien.
	Proc1 = fopen("Proceso1.txt","r");
	Proc2 = fopen("Proceso2.txt", "r");
	Proc3 = fopen("Proceso3.txt", "r");
	//getline(&linea,&buffer,&Proc1);

	return 0;
}
