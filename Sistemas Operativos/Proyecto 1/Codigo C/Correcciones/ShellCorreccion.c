#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>

#define MAX_BUF 1000


char *Comandos_Disponibles[] = {
	"makedir", //Crear directorio
	"remdir", //Eliminar Directorio
	"cat", //Crear archivo
	"more", //Mostrar contenido de un archivo
	"ls", //Listar contenidos de un directorio
	"help", //comando ayuda
	"exit", //salir
};

/*
 * función que simula el comportamiento del comando "mkdir"
 * EL cual crea un directorio con nombre "dn" 
*/
int makedir(char * dn){
int check;
	char *dirname = dn; //obtengo el nombre del directorio a crear

	check = mkdir(dirname);
	if(!check){
		printf("El directorio %s fue creado en la ruta actual con éxito\n",dirname);
	}
	else{
		fprintf(stderr,"Codigo de error %d, el cual significa %s. Ocurrió un error al crear el directorio %s \n",
					errno,strerror(errno),dn);
        return(-1);
	}

	return(0);

}

/*
 * función que simula el comportamineto del comando "rmdir"
 * el cual elimina un directorio
 * se asume que la carpeta se encuentra localizada dentro de donde esta este archivo fuente
*/
int remdir(char *name){
	int check;
	char *path = name;

    //ESTOY ASUMINEDO QUE EL DIRECTORIO YA ESTA VACIO PARA LLAMAR A LA FUNCION DE LIBRERIA!
    check = rmdir(path);
    if(check < 0){
        fprintf(stderr,"Codigo de error %d, el cual significa %s, ocurrió un error al borrar el directorio %s \n",
                errno,strerror(errno),path);
        return(-1);
    }
    else{
        printf("Se ha eliminado el directorio %s con exito\n",path);
        return(0);
    }

	return(0);

}
/*
 *función que simula el comportamiento de la función "more"
 * el cual lee un archivo de texto
*/
int more(char *name){
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
		return(-1);
	}
	printf("\n");
	return(0);
}

/*
 * Función que simula la ejecución del comando "cat", el cual crea un archivo
 * con un nombre pasado por parámetro
*/
int cat(char *name){

    FILE *file;
	file = fopen(name,"rb+"); //Abro el archivo para intentar ver si ya existe
	if(file != NULL){ //Si es != NULL, el archivo ya existia, esto tecnicamente es un error
		printf("Codigo de error -5: No se ha podido crear el archivo porque el mismo ya existe\n");
		return(-1);
	}
	else{ //Sino, intento crear el archivo
		file = fopen(name,"wb"); //Fuerzo la creación del archivo
		if(file == NULL){
			printf("Codigo de error -6: Error al crear el archivo especificado\n");
		}
		else{ //Si todo sale bien, cierro el archivo recien creado y reporto el exito de la operación
			fclose(file);
			printf("El archivo %s fue creado con exito\n",name);
			return(0);
		}
	}
	return(0);

}
/*
 * función que simula el comportamineto del comando "ls", el cual 
 * lista los contenidos de la carpeta donde se encuentra el codigo fuente
*/
int ls(){
    struct dirent *de;
	DIR *dr = opendir("."); //Abro la carpeta en donde estoy
	if(dr == NULL){
		fprintf(stderr,"Error -1: NO se ha podido abrir el Directorio");
		return (-1);
	}
	else{
		while((de = readdir(dr)) != NULL) //Mientras la carpeta leida no sea NULL
			printf("%s\n",de->d_name); //Obtengo el nombre de la carpeta y la imprimo por pantalla

		closedir(dr); //cierro el directorio
		return(0);
	}
	return(0);

}

/*
 * Método que verifica si el comando obtenido es alguno de los comandos disponibles
*/
int esLegal(char*str){
	bool es =  false;
	int i = 0;
	for(i = 0; i < 7 && !es; i++){
		if(strcmp(Comandos_Disponibles[i], str) == 0) { //Uso string Compare
			es = true;
			break;
		}
	}
	if(!es)
        i = -1;

	return i;
}

/*
 * Función que se encarga de  determinar que comando legal que requiere un paraetro fue ingresado
*/
int determinar1P(int i, char*param){ //Para makedir, remdir, cat o more
    int ret = 0;
    switch(i){

        case 0:
            ret = makedir(param);
            break;
        case 1:
            ret = remdir(param);
            break;
        case 2:
            ret = cat(param);
            break;
        case 3:
            ret = more(param);
			break;

    }

}

//Función que imprime el ensaje de ayuda
void help(){
	printf("Shell en C para el Primer Proyecto de Sistemas Operativos\n");
	printf("Ingrese alguno de los comandos disponibles con sus parametros correspondientes si los necesitase y presione ENTER\n");
	printf("A continuación se detallan los comandos disponibles:\n");
	printf("makedir [NombreDir] -> Crea un directorio con Nombre NombreDir en la posición actual\n");
	printf("remdir [NombreDir] -> Intenta Eliminar un directorio con NombreDir, solo podrá eliminarse si el directorio esta vacio\n");
	printf("cat [NombreAR] -> Intenta crear un Archivo con nombre NombreAR\n");
	printf("ls -> Lista los contenidos del directorio actual\n");
	printf("more [NombreAR] -> Muestra el contenido del archivo NombreAR\n");
	printf("exit -> termina la ejecución de esta consola\n");
	printf("help -> muestra el presente mensaje\n");
}

/*
 * Función que determina si el comando legal ingresado es alguno de los comandos
 * que NO requieren un parametro adicional
*/
int determinar0P(int i, bool *corte){ //Para ls, help o exit
    int ret = 0;
	switch(i){

		case 4:
			ret = ls();
			break;
		case 5:
			help();
			break;
		case 6:
			*corte = true;
			printf("Se ha salido de la consola con exito, que tenga un buen dia!\n");
			break;
		default: //No es ninguno de los 3
			ret = -2;
			break;
    }
    return ret;
}



int main(){

	char buffer[MAX_BUF]; //para obtener la linea completa
	char *token;
	char *name;
	bool corte = false; //boolean de corte para salir del programa
	bool error = false; //boolean para errores
	int errno;
	while(!corte){
		printf("pr1> ");
		scanf("%s", &buffer); //leo la linea, "pr1>" NO APARECE en strtok
		//Obtengo un substring, para ver que comando es
		token = strtok(buffer," ");
		int t = esLegal(token); //Determino si el comando ingresado es alguno de los permitidos
		if(t >= 0 ){ //Pregunto que el comando dado sea legal
			int E = 0;
			E = determinar0P(t, &corte);
			if (E == -2){ //Si no es ni ls, ni help, ni exit, entonces tengo algo con un parametro
				scanf("%s",&buffer);
				token = strtok(buffer," ");
				name = token;
				if(!corte) {
					//si llegué aquí, el comando Y tengo un parametro, puedo ir a crear el proceso hijo y ejecutar
					pid_t id = fork();
					if(id == -1) {
						printf("Error al crear el hijo\n");
					}
					if(id == 0) { //procedo a llamara a determinar1P, el cual luego llamará a alguna de las funciones de los comandos
						E = determinar1P(t,name);
						exit(E); //hago Exit con el estado que devolvió la función
					}
					if(id > 0) { //estoy en el padre, debo esperar
						wait(NULL);
					}
				}
			}
		}
		else{
			printf("Error al procesar el comando, NO es un comando valido, por favor, consulte la ayuda usando el comando help\n");
		}
	}
}
