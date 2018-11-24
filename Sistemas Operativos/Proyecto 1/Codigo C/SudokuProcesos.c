#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 27

/* Como seria la idea general para resolver el tester de Sudoku?
   1) generar la matriz -> El Padre lo hace
   		1.1)sabemos que la Matriz es de 9x9, asi que un arreglo bi-dimensional cuadrado de caracteres bastará
		1.2)Es necesario abrir el archivo de entrada (fopen()), leer caracter por caracter
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

*/

//Hago los archivos resultado globales para acceder facilmente
FILE *Proc1;
FILE *Proc2;
FILE *Proc3;

void Lectura(int GrillaSudoku[][9]){

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
                        int x = (int)(num - '0');
                        GrillaSudoku[F][C] = x; //si lo leido no es EOL o "," (la grilla esta separada por comas), lo añado a la matriz
                        C++;
                        printf("%d ",x);
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

/*Función adicional para chequear que  una vez llenada la matriz, la misma este completa, ya que si el input
	era incorrecto, puede haber lugares vacios
*/
bool Completitud(char GrillaSudoku [][9]){

	bool OK = true;
	int F;
	for(F = 0; F < 9 && OK; F++){
		int C;
		for(C = 0; C < 9 && OK; C++){
			/*Si porque habia un carcter que NO era un dígito, el lugar correspondiente en la Matriz quedó vacio,
				entonces la "jugada" no era válida*/
			if(GrillaSudoku[F][C] == NULL)
				OK = false;
		}
	}
	return OK;

}

/* Función que verificará un Cuadrante del Sudoku
*	gril, grilla del Sudoku
*	F, Fila de Inicio del cuadrante
*	al retornar, devuelve Verdadero (la fila es correcta) o Falso (la Fila NO es válida, aquí se debe terminar todo)
*/
void VerificarFila(char GrillaSudoku [][9], int F){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0};
    int I;
    for (I = 0; I < 9; I++) {
        int X = GrillaSudoku [F][I] - '0';
        if (X < 1 || X > 9) {
            printf("Valor ilegal en la fila\n");
            //Tengo un valor no posible
            return false;
        }
        if (Lista[X] == 1) {
            printf("Valor repetido en la Fila %d\n",F);
            //Me encuentro con un valor repetido
            return false;
        }
        Lista[X] = 1;
    }
    for (I = 1; I <= 9; I++) {
        //Verifico que esten todos los numeros
        if (Lista[I] == 0) {
            printf("Valor en  la fila %d faltante\n",F);
            //Faltaba un valor en la lista
            return false;
        }
    }
    printf("Fila %d Verificada con exito\n",F);
	return true;
}
/* Función que verificará una Columna del Sudoku
*   gril, grilla del Sudoku
*   C, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (La Columna es correcta) o Falso (La Columna NO es válida, aquí se debe terminar todo)
*/
void VerificarColumna(char GrillaSudoku [][9], int C){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0};
    int I;
    for (I = 0; I < 9; I++) {
        int X = GrillaSudoku[I][C] - '0';
        if (X < 1 || X > 9) {
            printf("Valor ilegal en la columna %d\n",C);
            //Tengo un valor no posible
            return false;
        }
        if (Lista[X] == 1) {
            printf("Lista[%d] = %d en I =%d\n",X,Lista[X],I);
            printf("Valor repetido en la columna %d\n",C);
            //Me encuentro con un valor repetido
            return false;
        }
        Lista[X] = 1;
    }
    for (I = 1; I <= 9; I++) {
        //Verifico que esten todos los numeros
        if (Lista[I] == 0) {
            printf("Valor en la Columna %d Faltante\n",C);
            //Faltaba un valor en la lista
            return false;
        }
    }
    printf("Columna %d verificada con exito\n",C);
    return true;
}

/* Función que verificará un Cuadrante del Sudoku
*   gril, grilla del Sudoku
*   X, Fila de Inicio del cuadrante
*   Y, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (El cuadrante es correcto) o Falso (El Cuadrante NO es válido, aquí se debe terminar todo)
*/
void VerificarCuadrante(char GrillaSudoku [][9], int X, int Y){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0}; //1 based, de 1 a 9
    int I;
    int J;
    for (I = X; I < X+3; I++) {
        for (J = Y; J < Y+3; J++) {
            int N = GrillaSudoku[I][J] - '0';
            if (N < 1 || N > 9) {
                //Tengo un valor no posible
                printf("Hay un valor ilegal en el cuadrante\n");
                return false; //usar fputs()
            }
            if (Lista[N] == 1) {
                printf("Habia un valor repetido en el cuadrante\n");
                //Me encuentro con un valor repetido
                return false;
            }
            Lista[N] = 1;
        }
    }
    for (I = 1; I <= 9; I++) {
        //Verifico que esten todos los numeros
        if (Lista[I] == 0) {
            //Faltaba un valor en la lista
            printf("Valor en la lista faltante para el cuadrante\n");
            return false;
        }
    }
    printf("Cuadrante %d,%d verificado con exito\n",X,Y);
    return true;
}

/*Método que, dependiendo del número pasado, discrimina la tarea a asignarle al proceso entrante
	si es el primer Proceso, le asigno que tiene que verificar las filas, si es el segundo, las columnas, sino,
	es el tercero y le asigno revisar los cuadrantes con sus respectivas funciones especiales dentro de un loop while
*/
void HacerTarea(int ProcNum, char GrillaSudoku [][9]){

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

int main(){
	
	Proc1 = fopen("Proceso1.txt","w");
	Proc2 = fopen("Proceso2.txt","w");
	Proc3 = fopen("Proceso3.txt","w");
	
	//Matriz de 9x9 donde se guardará el sudoku
	char GrillaSudoku [9][9];
	bool check1 = true;
	int pid = 0;

		Lectura(SudokuR, GrillaSudoku);

		if(Completitud(GrillaSudoku) != true){
			fprintf(stderr,"La jugada NO es correcta ya que el Input del mismo NO ERAN NÚMEROS EN SU TOTALIDAD\n");
			return 1;
		}
	



	/*
		Ciclo FOR que crea todos los procesos necesarios, luego, si estoy en el Hijo,
		Entro a un método especial que le asigna un trabajo a realizar
	*/
	int i;
	for(i=0; i<NUM_PROC && check1; i++){
    	pid=fork();
    	if(pid == -1){
    		fprintf(stderr,"Error al crear el Proceso Hijo Numero %d", i);
    		//tirar error, algo salió mal al crear el hijo
    	}
    	else{
    		if(pid != 0){
        		check1 = HacerTarea(i,GrillaSudoku); //Entro al método para saber que hacer yo como Hijo
        		//OJO, ver donde meter el exit() para terminar el hijo
        		exit(0); //una vez que yo, Proceso Hijo, termino mi tarea, hago Exit.
        		break;
    		}
    	}

	}
	if (pid == 0){ //Esty en el padre, debo esperar por mis 27 hijos
		int status;
		if(waitpid(pid,&status,0) == -1){
			printf("Error al esperar por el hijo %d\n",pid);
			return -1;
		}
		if(WIFEXITED(status)){
			const int es = WEXITSTATUS(status);
			if(es == -1)
				printf("El Proceso %d no pudo completar su tarea correctamente, la jugada de sudoku es invalida\n",pid);
		}
	}
	char res[100]; //Buffer para leer los archivos
	//Chequeo que todo esté bien, esto es, leo los 3 archivos y si todos tienen "true", entonces estaba todo bien.
	Proc1 = fopen("Proceso1.txt", "r");
	Proc2 = fopen("Proceso2.txt", "r");
	Proc3 = fopen("Proceso3.txt", "r");
	if(Proc1 && Proc2 && Proc3){
	    fscanf(Proc1,"%s",res1);
	    fscanf(Proc2,"%s",res2);
	    fscanf(Proc3,"%s",res3);
		if(res1 == "true" && res2 == "true" && res3 == "true"){
			printf("La jugada de Sudoku era Valida");
		}
		else{
			printf("La jugada de Sudoku NO era Valida");
		}
	}

	return 0;
}