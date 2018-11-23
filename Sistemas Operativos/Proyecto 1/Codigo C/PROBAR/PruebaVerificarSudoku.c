#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>



/* Función que verificará una Columna del Sudoku
*   gril, grilla del Sudoku
*   C, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (La Columna es correcta) o Falso (La Columna NO es válida, aquí se debe terminar todo)
*/
bool VerificarColumna(char GrillaSudoku [][9], int C){
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
*	gril, grilla del Sudoku
*	F, Fila de Inicio del cuadrante
*	al retornar, devuelve Verdadero (la fila es correcta) o Falso (la Fila NO es válida, aquí se debe terminar todo)
*/
bool VerificarFila(char GrillaSudoku [][9], int F){
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

/* Función que verificará un Cuadrante del Sudoku
*   gril, grilla del Sudoku
*   X, Fila de Inicio del cuadrante
*   Y, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (El cuadrante es correcto) o Falso (El Cuadrante NO es válido, aquí se debe terminar todo)
*/
bool VerificarCuadrante(char GrillaSudoku [][9], int X, int Y){
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

int main(){
    char Grilla[9][9]={ {'6','3','2','7','8','1','9','4','5'},
                        {'4','8','7','5','9','6','2','1','3'},
                        {'5','1','9','2','4','3','8','7','6'},
                        {'8','6','4','3','5','2','7','9','1'},
                        {'7','5','1','9','6','8','3','2','4'},
                        {'2','9','3','1','7','4','6','5','8'},
                        {'9','4','5','6','3','7','1','8','2'},
                        {'1','7','6','8','2','5','4','3','9'},
                        {'3','2','8','4','1','9','5','6','7'}  };

    int F,C;
    for(F = 0; F < 9; F++){
        printf("Verificando Fila numero %d = %d \n",F, VerificarFila(Grilla,0));    
        for(C = 0; C < 9; C++){
            printf("Verificando Columna numero %d = %d \n",C, VerificarFila(Grilla,0));
        }
    }
    F = 0;
    C = 0;
    while(F <= 6 && C <= 6){
        printf("Verificando cuadrante %d,%d\n" F,C,VerificarCuadrante(GrillaSudoku, rec, Y));
                F = F + 3;
                if(F == 6 && C != 6){
                    F = 0;
                    C = C + 3;
                }
    }
    return 0;
}
