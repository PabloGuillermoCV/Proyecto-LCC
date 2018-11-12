#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


char ALLOWED_OCTAL[6] = {'7','5','4','2','1','0'}; //Arreglo Global para saber los valores permitidos en octal para chmod
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera
//DEFINO TIPOS DE ERRORES QUE PUEDEN PRODUCIRSE AL PARSEAR LOS DATOS
#define ERR_EXTRA_PARAMS -1
#define ERR_INSUFFICIENT_PARAMS -2
#define ERR_WRONG_PERM -3


/*NOTAS:
	en Linux, los parámetros para chmod se pueden pasar tanto en caracteres, como en octal

	chmod options permissions filename

	u = user , g = group, o = other

	r = read , w = write, x = execute

	UNA LLAMADA LEGAL A chmod:

	chmod u=rwx,g=rx,o=r myfile -> de aquí se ve que la forma general de los parametros para chmod es "u=xyz,g=xyz,o=xyz myfile" 

	igual llamada, pero en Octal: chmod 754 myfile

	chmod a=rw myfile -> permite A TODOS LOS USUARIO leer y escribir el archivo myfile

	en las llamadas que yo uso de ejemplos NO considero el "Sticky Bit", el cual es un cuarto valor en modo octal

		ejemplo -> chmod 1777 myfile

	por ende:
		1) HAY CONTROLAR QUE EL COMANDO USE UNA DE LAS DOS REPRESENTACIONES
		2) EL ORDEN IMPORTA EN MODO CARACTER!
		3) UNA VEZ VERIFICADO QUE LA SECCIÓN DE PERMISOS ES CORECTA, SE HACE LA LLAMADA A CHMOD 

Código original de la llamada a chmod:
	fprintf(stderr, "%s: error en chmod(%s, %s) - %d (%s)\n",
                argv[0], buf, mode, errno, strerror(errno));
        exit(1);

*/

/*
 * Función para explicitar los mensajes de error propios asociados a la ejecución de este programa
*/
char* MensError(int n){

	char *ret;

	switch(n){
		case -1:
			ret = "Los parametros presentados sobran\n";
			break;
		case -2:
			ret = "NO hay parametros suficientes para la llamada a chmod\n";
			break;
		case -3:
			ret = "Codigo de permisos en sistema de representacion Octal ilegal\n";
			break;
	}

	return ret;
}

/*
 * Método para determinar que un número pertenezca a los valores permitidos en Octal de chmod
*/
int PerteneceNums(char n){
	int i;
	int ret = 0;
	for(i = 0; i < NELEMS(ALLOWED_OCTAL) && ret; i++){
		if(n == ALLOWED_OCTAL[i]){
			ret = 1;  //El numero obtenido es un octal legal para chmod
		}
	}
	if(ret != 1)
		ret = ERR_WRONG_PERM; //Codigo de error propio, "Código de permisos Ilegal"

	return ret;
}

/*
 * Método con el cual intento corroborar que la llamada EN OCTAL de chmod sea correcta
*/
int LeerOctal(char *Perms){
	int ret = 1;
	int i;
	if (sizeof(Perms) > 4)
		ret = ERR_EXTRA_PARAMS; //Código de error propio, como hay más parámetros de los permitidos 
	else{
		if(sizeof(Perms) < 4)
			ret = ERR_INSUFFICIENT_PARAMS; //Código de error propio "Cantidad de permisos a setear insuficiente, debe ser del tipo ugo"
		else{
			for(i = 0; i < (sizeof(Perms) - 1) && ret == 1; i++){
				ret = PerteneceNums(Perms[i]);
			}		
		}
	}
	return ret;
}



int main(int argc, char * argv[]){

	char *Permisos;
	char *FILE;
	int errno; //entero para códigos de error, sean propios o producto de la llamada a chmod

	if(argc != 3){ //ERROR, me pasaron una cantidad insuficiente o de más de argumentos
		printf("ERROR: NO se han especificado la cantidad de argumentos correctos para la llamada a chmod\n");
		printf("Por favor, ingrese una linea de la forma <permisos> <Nombre_Arcivo> de acuerdo a los parametros de la funcion chmod\n");
		exit(1);
	}
	else{
		Permisos = argv[1]; 
		FILE = argv[2];
		if(PerteneceNums(Permisos[0]))
				errno = LeerOctal(Permisos); //Chequear resto de la sintaxis de la entrada en octal
		else{ //Si no entra, me mandaron cualquier cosa, abortar y reportar
			fprintf(stderr, "La llamada que se esta intentando hacer NO coincide con la llamada a chmod");
			exit(1);
		}

		if(errno == 1){
			int p = strtol(Permisos,0,8); //Hago la llamada a Sting to Long() para transformar la linea de los permisos en un número que pueda leer chmod()
			//Si llegué hasta aquí, el control fue correcto, puedo invocar a chmod
			errno = (int) chmod (FILE,p); 
			 if (errno < 0){ //
	        	fprintf(stderr, "%s: error en la llamada chmod(%s, %s) - %d (%s)\n",
	                	argv[0], FILE, Permisos, errno, strerror(errno));
	        	exit(1);
	    	}
	    	else{
	    		printf("Se ha podido ejecutar chmod con exito\n");
	    	}
	    	return(0);
    	}
    	else{
    		fprintf(stderr, "error en el analisis de los datos suministrados con codigo %d, lo que significa: %s \n", errno, MensError(errno));
    		exit(1);
    	}
	}
}