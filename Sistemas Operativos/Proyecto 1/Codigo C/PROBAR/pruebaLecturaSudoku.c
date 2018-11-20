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
	FILE *SudokuR = fopen("sudoku.txt","r");
	while(!feof(SudokuR) && F < 9 && !corte){
		printf("Fila Nro: %d = ",F);
		while(C < 9 && !corte){
			num = fgetc(SudokuR); //obtengo el caracter
			if((int)num != 44 && (int)num < 48 && (int)num > 57)
				corte = true;
			if(num != ',' && num != '\n'){
				int x = num - '0';
				GrillaSudoku[F][C] = x; //si lo leido no es EOL o "," (la grilla esta separada por comas), lo añado a la matriz
				C++;
				printf("%d ",x);
			}
		}
		printf("\n");
		F++;
		C = 0;
	}
	if(corte)
		printf("Error al procesar la entrada, %c NO es una entrada valida para un sudoku",num);
}

int main(){
	int Grilla[9][9];
	int F;
	int C;
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