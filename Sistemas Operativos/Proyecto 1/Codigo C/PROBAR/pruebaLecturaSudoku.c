#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


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

int main(){
	//NOTA: se puede hacer Grilla [9][9] = {{0}} para inicializar la matriz entera en 0
	//int array[1024] = {[0 ... 1023] = 5}; tambien sirve
	int Grilla[9][9] = {{0}};
	int F;
	int C;
	printf("Entrar a Lectura\n");
	Lectura(Grilla);
	for(F = 0; F < 9; F++){
		for(C = 0; C < 9; C++){
			printf(" %d ",Grilla[F][C]);
		}
		printf("\n");
	}
	printf("Fin lectura sudoku\n");
	return 0;
}
