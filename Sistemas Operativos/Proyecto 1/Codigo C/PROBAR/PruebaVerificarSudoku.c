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
    bool Lista [9];
    int I;
    for (I = 0; I < 9; I++) {
        int X = GrillaSudoku[I][C] - '0';
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
*	F, Fila de Inicio del cuadrante
*	al retornar, devuelve Verdadero (la fila es correcta) o Falso (la Fila NO es válida, aquí se debe terminar todo)
*/
bool VerificarFila(char GrillaSudoku [][9], int F){
    bool Lista [9];
    int I;
    for (I = 0; I < 9; I++) {
        int X = GrillaSudoku [F][I] - '0';
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
*   gril, grilla del Sudoku
*   X, Fila de Inicio del cuadrante
*   Y, Columna de Inicio del Cuadrante
*   al retornar, devuelve Verdadero (El cuadrante es correcto) o Falso (El Cuadrante NO es válido, aquí se debe terminar todo)
*/
bool VerificarCuadrante(char GrillaSudoku [][9], int X, int Y){
    bool Lista [9];
    int I;
    int J;
    for (I = X; I < X+3; I++) {
        for (J = Y; J < Y+3; J++) {
            int N = GrillaSudoku[I][J] - '0';
            if (N < 1 || N > 9) {
                //Tengo un valor no posible
                return false; //usar fputs()
            }
            if (Lista[N] == true) {
                //Me encuentro con un valor repetido
                return false;
            }
            Lista[N] = true;
        }
    }
    for (I = 0; I < 9; I++) {
        //Verifico que esten todos los numeros
        int N = I + 1;
        if (Lista[N] == false) {
            //Faltaba un valor en la lista
            return false;
        }
    }
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

    printf("Verificando Fila numero 1 = %d \n", VerificarFila(Grilla,0));
    printf("Verificando Columna Numero 1 = %d \n",VerificarColumna(Grilla,0));
    printf("Verificando Cuadrante Numero 1 = %d \n",VerificarCuadrante(Grilla,0,0));

    return 0;
}