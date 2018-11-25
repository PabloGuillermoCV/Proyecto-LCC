#include <stdio.h>
#include <string.h>
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
		2.1)la idea seria usar 27 procesos distintos y hacer que el Padre espere a los 27 hijos
			2.1.1) Procesos que verifiquen las Filas del Sudoku
				Este verificará fila por fila que en esa fila esten los números del 1 al 9 (SIN REPETICIÓN)
			2.1.2) Procesos que verifiquen las Columnas del Sudoku
				Este verificará Columna por Columna que en esa columna esten los números del 1 al 9 (SIN REPETICIÓN)
			2.1.3) Procesos que verifiquen los cuadrantes 3x3 del Sudoku
				Este verificará por Cuadrante (hay 9 cuadrantes, empezando en las posiciones (0,0), (0,3)  (0,6)
				(3,0), (3,3), (3,6) , (6,0) , (6,3) , (6,6))
		2.2) Estos Procesos deben reportar si su tarea falló en caso de que la jugada sea inválida, al primero que lo hace, se
			 debe cortar TODO, el padre debe matar a los hijos restantes
		2.3) si NINGUNO falla y TODOS terminan, entonces la jugada de Sudoku era válida, entonces, el Padre reporta que la jugada era válida

*/

int Cuadrantes[18] = {0,0,0,3,0,6,3,0,3,3,3,6,6,0,6,3,6,6}; //Arreglo global de cuadrantes


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
                    }
                }
            }
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
/* Me parece innecesario
bool Completitud(char GrillaSudoku [][9]){

	bool OK = true;
	int F;
	for(F = 0; F < 9 && OK; F++){
		int C;
		for(C = 0; C < 9 && OK; C++){
			//Si porque habia un carcter que NO era un dígito, el lugar correspondiente en la Matriz quedó vacio,
			//entonces la "jugada" no era válida
			if(GrillaSudoku[F][C] == NULL | GrillaSudoku[F][C] == ' ')
				OK = false;
		}
	}
	return OK;

}
*/

/* Función que verificará un Cuadrante del Sudoku
*	gril, grilla del Sudoku
*	F, Fila de Inicio del cuadrante
*/
int VerificarFila(char GrillaSudoku [][9], int F,char *nombre){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0};
    int I;
    FILE *res = fopen(nombre,"wb");
    if(res != NULL){
        for (I = 0; I < 9; I++) {
            int X = GrillaSudoku [F][I] - '0';
            if (X < 1 || X > 9) {
                printf("Valor ilegal en la fila\n");
                //Tengo un valor no posible
                fputc(-1,res);
                return 0;
            }
            if (Lista[X] == 1) {
                printf("Valor repetido en la Fila %d\n",F);
                //Me encuentro con un valor repetido
                fputc(-1,res);
                return 0;
            }
            Lista[X] = 1;
        }
        for (I = 1; I <= 9; I++) {
            //Verifico que esten todos los numeros
            if (Lista[I] == 0) {
                printf("Valor en  la fila %d faltante\n",F);
                //Faltaba un valor en la lista
                fputc(-1,res);
                return 0;
            }
        }
        printf("Fila %d Verificada con exito\n",F);
        fputc(0,res);
        fclose(res);
    }
    else{
        printf("Error al crear el archivo de escritura\n");
        return -1;
    }

	return 0;
}
/* Función que verificará una Columna del Sudoku
*   gril, grilla del Sudoku
*   C, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (La Columna es correcta) o Falso (La Columna NO es válida, aquí se debe terminar todo)
*/
int VerificarColumna(char GrillaSudoku [][9], int C,char *nombre){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0};
    int I;
    FILE *res = fopen(nombre,"wb");
    if(res != NULL){
        for (I = 0; I < 9; I++) {
            int X = GrillaSudoku[I][C] - '0';
            if (X < 1 || X > 9) {
                printf("Valor ilegal en la columna %d\n",C);
                //Tengo un valor no posible
                return -1;
            }
            if (Lista[X] == 1) {
                printf("Lista[%d] = %d en I =%d\n",X,Lista[X],I);
                printf("Valor repetido en la columna %d\n",C);
                //Me encuentro con un valor repetido
                return -1;
            }
            Lista[X] = 1;
        }
        for (I = 1; I <= 9; I++) {
            //Verifico que esten todos los numeros
            if (Lista[I] == 0) {
                printf("Valor en la Columna %d Faltante\n",C);
                //Faltaba un valor en la lista
                return -1;
            }
        }
        printf("Columna %d verificada con exito\n",C);
        fputc(0,res);
        fclose(res);
    }
    else{
        printf("Error al abrir el archivo res para escribir\n");
        return -1;
    }
    return 0;
}

/* Función que verificará un Cuadrante del Sudoku
*   gril, grilla del Sudoku
*   X, Fila de Inicio del cuadrante
*   Y, Columna de Inicio del Cuadrante
*/
int VerificarCuadrante(char GrillaSudoku [][9], int X, int Y,char *nombre){
    int Lista [10] = {0,0,0,0,0,0,0,0,0,0}; //1 based, de 1 a 9
    int I;
    int J;
    FILE *res = fopen(nombre,"wb");
    if(res != NULL){
        for (I = X; I < X+3; I++) {
            for (J = Y; J < Y+3; J++) {
                int N = GrillaSudoku[I][J] - '0';
                if (N < 1 || N > 9) {
                    //Tengo un valor no posible
                    printf("Hay un valor ilegal en el cuadrante\n");
                    fputc(-1,res);
                    return 0; //usar fputs()
                }
                if (Lista[N] == 1) {
                    printf("Habia un valor repetido en el cuadrante\n");
                    fputc(-1,res);
                    //Me encuentro con un valor repetido
                    return 0;
                }
                Lista[N] = 1;
            }
        }
        for (I = 1; I <= 9; I++) {
            //Verifico que esten todos los numeros
            if (Lista[I] == 0) {
                //Faltaba un valor en la lista
                printf("Valor en la lista faltante para el cuadrante\n");
                fputc(-1,res);
                return -1;
            }
        }
        printf("Cuadrante %d,%d verificado con exito\n",X,Y);
        fputc(0,res);
        fclose(res);
    }
    else{
        printf("Error al crear el archivo para escribir\n");
        return -1;
    }

    return 0;
}

/*Método que, dependiendo del número pasado, discrimina la tarea a asignarle al proceso entrante
	si es el primer Proceso, le asigno que tiene que verificar las filas, si es el segundo, las columnas, sino,
	es el tercero y le asigno revisar los cuadrantes con sus respectivas funciones especiales dentro de un loop while
*/
int HacerTarea(int ProcNum, char GrillaSudoku [][9]){

	int rec,Y,res = 0;
    char nombre[27];
    sprintf(nombre,"%d.txt",ProcNum); //paso el Procnum a un string para hacer el nombre del archivo
    //nombre = strcat(nombre,".txt");
    printf("%s\n",nombre);
	switch(ProcNum){

		case 0 ... 8:
            res = VerificarFila(GrillaSudoku, ProcNum,nombre);
			break;
		case 9 ... 17:
            res = VerificarColumna(GrillaSudoku,(ProcNum-9),nombre);
			break;
		case 18 ... 26:
            res = VerificarCuadrante(GrillaSudoku, Cuadrantes[(ProcNum-18)], Cuadrantes[(ProcNum-17)],nombre);
			break;
	}
	return res;
}

int main(){


	//Matriz de 9x9 donde se guardará el sudoku
	char GrillaSudoku [9][9];
	bool check = true;
	int pid = 0;

		Lectura(GrillaSudoku);

		/*
		if(Completitud(GrillaSudoku) != true){
			fprintf(stderr,"La jugada NO es correcta ya que el Input del mismo NO ERAN NÚMEROS EN SU TOTALIDAD\n");
			return 1;
		}*/




	/*
		Ciclo FOR que crea todos los procesos necesarios, luego, si estoy en el Hijo,
		Entro a un método especial que le asigna un trabajo a realizar
	*/
	int i;
	for(i=0; i < NUM_PROC; i++){
    	pid=fork();
    	if(pid == -1){
    		fprintf(stderr,"Error al crear el Proceso Hijo Numero %d\n", i);
    		//tirar error, algo salió mal al crear el hijo
    	}
    	else{
    		if(pid != 0){
        		int E = HacerTarea(i,GrillaSudoku); //Entro al método para saber que hacer yo como Hijo
        		//OJO, ver donde meter el exit() para terminar el hijo
        		exit(E); //una vez que yo, Proceso Hijo, termino mi tarea, hago Exit.
        		break;
    		}
    	}

	}
	if (pid == 0){ //Esty en el padre, debo esperar por mis 27 hijos
		wait(NULL);
		/*if(waitpid(pid,&status,0) == -1){ VER ESTO
			printf("Error al esperar por el hijo %d\n",pid);
			return -1;
		}
		if(WIFEXITED(status)){
			const int es = WEXITSTATUS(status);
			if(es == -1)
				printf("El Proceso %d no pudo completar su tarea correctamente, la jugada de sudoku es invalida\n",pid);
		}*/
	}

	//Chequeo que todo esté bien, esto es, leo los 27 archivos y si todos tienen "0", entonces estaba todo bien.
    int num;
	for(i = 0; i < NUM_PROC && check; i++){
        char *nombre;
        sprintf(nombre,"%d",i);
		FILE *r = fopen(nombre,"r");
		if(r != NULL){
			fscanf(r,"%d",&num);
			if(num == 0){
                fclose(r);
				num = 2; //Pongo un valor distinto para evitar colisiones de respuestas
			}
			else{
				check = false;
				fclose(r);
				printf("La jugada de sudoku NO era valida\n");
			}
		}
		else{
			printf("Ocurrió un error al abrir el archivo de respuesta %d, este proceso fallío en dar una respuesta\n",i);
		}
	}

    if(check)
        printf("La jugada de sudoku era valida\n");

	return 0;
}
