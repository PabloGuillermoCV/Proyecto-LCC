#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcnt1.h>
#include <bool.h>
#include <string.h>
#include <errno.h>


char ALLOWED_OCTAL[6] = {7,5,4,2,1,0} //Arreglo Global para saber los valores permitidos en octal para chmod
char ALLOWED_PERMS[3] = {'r','w','x'} //Arreglo Global para saber los caracteres legales PARA LOS PERMISOS
char ALLOWED_GROUPS[4] = {'u','g','o','a'} //Arreglo Global para saber los caracteres legales PARA LOS USUARIOS
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0])) //Macro que calcula la cantidad de elementos presentes en un arreglo cualquiera


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
	fprintf(stderr, "%s: error in chmod(%s, %s) - %d (%s)\n",
                argv[0], buf, mode, errno, strerror(errno));
        exit(1);

*/

/*
 * Método para determinar que un número pertenezca a los valores permitidos en Octal de chmod
*/
int PerteneceNums(char n){
	int i;
	int a = (int)n; 
	int ret = 0;
	for(i = 0; i < NELEMS(ALLOWED_OCTAL) && ret == 0; i++){
		if(!(a == (int)(ALLOWED_OCTAL[i]))){
			ret = -3; //Codigo de error propio, "Código de permisos Ilegal"
		}
	}
	return ret;
}

/*
 * Método con el cual intento corroborar que la llamada EN OCTAL de chmod sea correcta
*/
int LeerOctal(char *Perms){
	int ret = 0;
	int i;
	if (sizeof(Perms) > 3)
		ret = -1; //Código de error propio, como hay más parámetros de los permitidos (OJO, NO CONSIDERO STICKY BIT, PREGUNTAR), corto y guardo esto como código de error 
	else{
		if(sizeof(Perms) < 3)
			ret = -2; //Código de error propio "Cantidad de permisos a setear insuficiente, debe ser del tipo ugo"
		else{
			for(i = 1; i < sizeof(Perms) && ret == 0; i++){
				ret = PerteneceNums(Perms[i]);
			}		
		}
	}
	
	return ret;
}

/*
 * Método con el cual determino si un caracter dado esta dentro de los permitidos PARA LOS PERMISOS de chmod
*/
int perteneceChar(char c){
	int i;
	int ret = 0;
	for(i = 0; i < NELEMS(ALLOWED_PERMS) && ret == 0; i++){
		if(!(c == ALLOWED_PERMS[i]))
			ret = -4; //Codigo de error propio "Permiso en caracteres incorrecto"
	}
	return ret;
}

/*
 * Método con el cual intento corroborar que la llamada POR STRING de chmod sea correcta 
*/
int LeerChars(char *Perms){
	int ret = 0;
	int i = 0;
	char comp = 'a';
	if(Perms[0] == comp){
		if(Perms[1] == '='){
			for(i = 2; i < sizeof(Perms) && ret == 0; i++){
				if(comp != Perms[i]) //Pregunto que NO se me estén repitiendo permisos (chmod no permite a=wrr, por ejemplo), la primera vez debe entrar
					ret = perteneceChar(Perms[i]); //de ahi en más, se que los permisos son del tipo a=<algo>, tengo que corroborar que cada caracter de <algo> sea legal
				comp = Perms[i]; //guardo el caracter usado para ver luego que no se me repita
			}
		}
	}
	else{
		if(Perms[0] == 'u'){
			comp = 'u';
		}
		else{
			ret = -5; //codigo de error propio "Usuario Incorrecto para los permisos"
		}
	}

}


int main(int argc, const char * argv[]){

	char *Permisos;
	char *FILE;
	int errno; //entero para códigos de error, sean propios o producto de la llamada a chmod

	if(argc == 1 | argc == 2 | argc > 3){ //ERROR, me pasaron una cantidad insuficiente o de más de argumentos
		printf("ERROR: NO se han especificado la cantidad de argumentos correctos para la llamada a chmod\n");
		printf("Por favor, ingrese una linea de la forma <permisos> <Nombre_Arcivo> de acuerdo a la funcion chmod\n");
		exit(1);
	}
	else{
		Permisos = argv[1]; 
		FILE = argv[2];
		if(PerteneceNums(Permisos[0]))
				LeerOctal(Permisos);
		else
			LeerChars(Permisos);
		//Si llegué hasta aquí, el control fue correcto, puedo invocar a chmod
		errno = chmod (FILE,Permisos); 
		 if (errno < 0){ //
        	fprintf(stderr, "%s: error en la llamada chmod(%s, %s) - %d (%s)\n",
                	argv[0], buf, mode, errno, strerror(errno));
        	exit(1);
    	}
    	return(0);
	}



}