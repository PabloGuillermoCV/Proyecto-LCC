#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
//ls
#include <dirent.h>

#define MAX_BUF 1000


char *Comandos_Disponibles[] = {
	"makedir", //Crear directorio
	"remdir", //Eliminar Directorio
	"cat", //Crear archivo
	"more", //Mostrar contenido de un archivo
	"ls", //Listar contenidos de un directorio
	"exit", //salir
	"help" //comando ayuda
};

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

int remdir(char *name){
	int check;
	DIR *d;
	char *path = name;
	d = opendir(path);
	if(d == NULL){
		printf("Error -3: El directorio dado NO se encuentra en las inmediaciones\n");
		return(-1);
	}
	else{
		closedir(d); //ANTES de continuar, cierro el directorio
		//ESTOY ASUMINEDO QUE EL DIRECTORIO YA ESTA VACIO PARA LLAMAR A LA SYSCALL!
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
	}
	return(0);

}

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
	return(0);
}

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

int determinar0P(int i){
	ret = 0;
	switch(i){
		
		case 4:
			ret = ls();
			break;
		case 5:
			corte = true;
			printf("Se ha salido de la consola con exito, que tenga un buen dia!\n");
			break;
		case 6:
			help();
			break;
		default:
			ret = -2;
			break;
		
	}
}

int determinar1P(int i, char*param){
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

    }

}

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
		int t = esLegal(token);
		if(t >= 0){ //Pregunto que el comando dado sea legal
			int E = determinar0P(t);
			if (E == -2){
				scanf("%s",&buffer);
				token = strtok(buffer," ");
				name = token;
				if(!corte) {
					//si llegué aquí, el comando Y los flags son validos y tengo un parametro, puedo ir a crear el proceso hijo y ejecutar
					pid_t id = fork();
					if(id == -1) {
						printf("Error al crear el hijo\n");
					}
					if(id == 0) { //procedo a llamara a determinar, el cual luego llamará a alguna de las funciones de los comandos
						E = determinar1P(t,name);
						exit(E);
					}
					if(id > 0) { //estoy en el padre, debo esperar
						wait(NULL);
					}
				}
			}
			
			/*if(strcmp(token,"help") == 0){ //si es "help" o "exit" actuo acordemente
				help();
			}
			else {
				if(strcmp(token,"exit") == 0){
					corte = true;
					printf("Se ha salido de la consola con exito, que tenga un buen dia!\n");
				}
				else{
					if (strcmp(token,"ls") == 0) {
						ls();
					}
					else {
						scanf("%s",&buffer);
						token = strtok(buffer," ");
						name = token;
						
						if(!corte) {
							//si llegué aquí, el comando Y los flags son validos y tengo un parametro, puedo ir a crear el proceso hijo y ejecutar
							pid_t id = fork();
							if(id == -1) {
								printf("Error al crear el hijo\n");
							}
							if(id == 0) { //procedo a llamara a determinar, el cual luego llamará a alguna de las funciones de los comandos
								int E = determinar(t,name);
								exit(E);
							}
							if(id > 0) { //estoy en el padre, debo esperar
								wait(NULL);
							}
						}
					}
				}
			}*/
		}
		else{
			printf("Error al procesar el comando, NO es un comando valido, por favor, consulte la ayuda usando el comando help\n");
		}
	}
}