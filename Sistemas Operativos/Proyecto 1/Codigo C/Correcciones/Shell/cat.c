#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "Params.h"

int main(){

	args *Par;
	char *name = Par->nombre;
	FILE *file;
	file = fopen(name,"rb+"); //Abro el archivo para intentar ver si ya existe
	if(file != NULL){ //Si es != NULL, el archivo ya existia, esto tecnicamente es un error
		printf("Codigo de error -5: No se ha podido crear el archivo porque el mismo ya existe\n");
	}
	else{ //Sino, intento crear el archivo
		file = fopen(name,"wb"); //Fuerzo la creación del archivo
		if(file == NULL){
			printf("Codigo de error -6: Error al crear el archivo especificado\n");
		}
		else{ //Si todo sale bien, cierro el archivo recien creado y reporto el exito de la operación
			fclose(file);
			printf("El archivo %s fue creado con exito\n",name);
		}
	}
	return 0;
}