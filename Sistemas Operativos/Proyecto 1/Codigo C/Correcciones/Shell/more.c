#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "Params.h"

/*
 * FUnción auxiliar para leer un archivo
 * usa un bufer de 255 caracteres
*/
void leer(char *name){
	FILE *f = fopen(name,"r"); //Abro el archivo en modo lectura
	char buff[255];
	if(f != NULL){ //Si la apertura fue correcta, comienzo a leer
		while(!feof(f)){
			fgets(buff,255,(FILE*)f);
			printf("%s",buff);
		}
		fclose(f); //terminada la lectura, cierro el archivo
	}
	else{ //Si no pude abrir el archivo, tiro error
		printf("codigo de error -4: Error al abrir el archivo especificado\n");
	}
}

int main(){

	args *Par;
	char *nom = Par->nombre; //Obtengo el nombre del archivo a leer con mi estructura compartida (PREGUNTAR)
	leer(nom);
	return 0;
}