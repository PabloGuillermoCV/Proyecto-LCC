#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

#define MAX_LINE 80 //Largo máximo para un comando
#define TOK_BUFSIZE 64 //Tamaño del buffer usado por strtok
#define TOK_DELIM " \t\r\n\a" //Delimitadores legales para strtok

char *Comandos_Disponibles[]{
	"mkdir" //Crear directorio
	"rmdir" //Eliminar Directorio
	"cat" //Crear archivo / Mostrar el contenido de un archivo
	"ls" //Listar contenidos de un directorio
	"more" //Mostrar contenido de un archivo
	"exit" //salir
	"help" //comando ayuda
}

char *Comandos_Predeterminados[]{
	"help"
	"exit"
}

int num_Predeterminados(){
	return sizeof(Comandos_Predeterminados) / sizeof(char*);
}

int help(** args){
	printf("Shell en C para el Primer Proyecto de Sistemas Operativos\n");
	printf("Ingrese alguno de los comandos disponibles y presione ENTER\n");
	printf("A continuación se detallan los comandos disponibles\n");
	printf("mkdir [NombreDir] Crea un directorio con Nombre NombreDir en la posición actual\n");
	printf("rmdir [NombreDir] Intenta Eliminar un directorio con NombreDir, solo podrá eliminarse si el directorio esta vacio\n")
	printf("cat [NombreAR] Intenta crear un Archivo con nombre NombreAR\n");
	printf("ls [Args] Lista los contenidos del directorio actual, dependiendo de los Args dados\n");
	printf("more [NombreAR] Muestra el contenido del archivo NombreAR\n");
	printf("exit termina la ejecución de esta consola\n");
	printf("help muestra el presente mensaje\n");
}

int exit(** args){
	//esta función deberia terminar todo
	return 0;
}

void loop(void){
	char *linea; //Linea del Shell
	char **args; //Argumentos de la Linea dada
	int Estado; //Estado actual para saber si se debe terminar o no

	//Se hace un Do-While para ciclar una vez inicialmente
	do{
		printf("pr1>"); //imprimo una linea para nuestro shell
		linea = Leer_Linea(); //Primer función, esta lee la linea ingresada
		args = Separar_Argumentos(linea); //Segunda función, dada la linea, busca los argumentos y los separa
		Estado = lanzar(args); //Tercer Función, dados los argumentos, ejecuta el comando

		free(linea); //Libero el espacio
		free(args);

	}while(Estado);
}

/*Primer Función principal cuya función es leer la linea
*/
void *Leer_Linea(){

	char *linea = NULL;
	ssize_t buffer = 0;
	getline(&linea,&buffer,stdin); //dependo de la función getline(), esta obtiene la linea desde el stdin, asignandole un buffer
	return linea;
}

/*Segunda Función principal cuya función es, dada la linea
	Separar en Comando y Argumentos
  ASUME QUE LOS ARGUMENTOS ESTAN SEPARADOS POR ESPACIOS, NO HAY COMAS NI NADA
*/
void **Separar_Argumentos(char *linea){
	int size_Buffer = TOK_BUFSIZE;
	int Pos = 0;
	char **tokens = malloc(size_Buffer* sizeof(char*)); //"Arreglo" de Tokens
	char *token; //Token simple usado para leer la linea de a partes

	if(!tokens){
		//Ocurrió un error al crear el arreglo, tiro error
		fprintf(stderr, "Error de locación\n");
	}
	/*Función strtok, lo hace es, dado un String y un delimitador, sistematicamente separar las lineas en sublineas
		separadas por el delimitador dado
		lo que hago aquí es sistematicamente separar la linea y cada "token" enviarlo a una posición especifica
			del arrego de "Tokens", entonces, el primer "Token" SIEMPRE será el comando, y el resto serán los argumentos

	*/
	token = strtok(linea, TOK_DELIM);
	while(token != NULL){
		tokens[Pos] = token;
		Pos++;
		/*Que pasa? tokens es en realidad un Arreglo dinámico, entonces,
			si por alguna razón la linea es demasiado larga, yo puedo darle más espacio a tokens para seguir leyendo
		*/
		if(Pos >= size_Buffer){
			size_Buffer += TOK_BUFSIZE;
			tokens = realloc(tokens, size_Buffer * sizeof(char*));
			if(!tokens){
				//ocurrió un error al alocar espacio
				fprintf(stderr, "Error en el buffer de lectura");
				exit(EXIT_FAILURE);
			}
			//hago un strtok con NULL para terminar
			token = strtok(NULL, TOK_DELIM);
		}
	}
	//Digo que la útima posición de tokens es un nulo para terminar
	tokens[Pos] = NULL;
	return tokens;
}
int lanzar(char **args){
	if(args[0] == NULL){
		//NO se ingresó nada, simplemente salir y volver a esperar stdin
		return 1;
	}

	for(int i = 0; i < num_Predeterminados() ; i++){
		/*Si el comando ingresado es alguno de los comandos predeterminados
			simplemente lo ejecuto yo con una función propia, sino, delego en Ejecutar(args) que creará el hijo
				para hacer todo el tramite
		*/
		if(strcmp(args[0], Comandos_Predeterminados[i]) == 0){
			return (*Comandos_Predeterminados[i])(args);
		}
	}
 return Ejecutar(args);
}



/*Tercer Función principal cuya función es, dado los argumentos, ejecutar el comando
	OJO ACÁ, estoy asumiendo que me estan ingresando un comando válido
		hay que hacer un chequeo adicional con Comandos_Disponibles[]
*/
int Ejecutar(char **args){

	int PID, WPID, Estado;

	PID = fork();
	if(PID == 0){
		//Estoy en el proceso Hijo, uso execvp para lanzar el proceso
		if(execvp(args[0], args) == -1){
			//tirar error
		}
		exit(EXIT_FAILURE);
	}
	else{
		if(PID < 0){
			fprintf(stderr, "Error al crear el Proceso Hijo\n");
			//Error al hacer Fork, reportar

		}
		else{
			//Estoy en el proceso Padre, espero a mi hijo
			do{
				WPID = waitpid(PID,&Estado,WUNTRACED);
			}while (!WIFEXITED(Estado) && WIFSIGNALED(Estado));
		}
	}
	return 1;
}

int main(void){


		loop();

		/*	Como seguir?
		*
		*	1) Definir los comandos pedidos por el proyecto
				1.1)cada comando deberia tener un pseudónimo asociado
				1.2)cada comando deberia rederigir a una función/procedimiento
				1.3)definir Argumentos legales para la shell y sus modificadores (archivos, '&' , '|', '-x' , etc)
			2) Hacer que la shell lea el input dado
				2.1) Debe darse cuenta que comando fue el llamado y NUESTRO Shell debe PROCESAR los modificadores asociados
				2.2) Hecho esto, se delega en el System Call Correspondiente (mkdir, deldir, etc)
					2.2.1) el shell debe crear un hijo con Fork(), y luego el hijo debe ejecutar execvp(char *command, char *params[])
			3) Consideraciones
				3.1) cuando el usuario ingrese "exit", debe_correr se setea a 0 y el programa debe terminar
				3.2) char *argumentos guardará el contenido del comando ingresado por pantalla, un lugar por "token"
					(1 token es el nombre del comando, otro token es un modificador como "-la", etc)
		*/

}
