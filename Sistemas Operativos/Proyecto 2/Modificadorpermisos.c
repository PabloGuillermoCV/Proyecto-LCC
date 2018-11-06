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
//DEFINO TIPOS DE ERRORES QUE PUEDEN PRODUCIRSE AL PARSEAR LOS DATOS
#define ERR_EXTRA_PARAMS -1
#define ERR_INSUFFICIENT_PARAMS -2
#define ERR_WRONG_PERM -3
#define ERR_INCORRECT_PERM -4
#define ERR_WRONG_USER -5
#define ERR_INVALID_USER_TYPE -6
#define ERR_WRONG_LITERAL -7
#define ERR_REPEAT_PERM -8


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
		case -4:
			ret = "El caracter de permisos ingresado NO es correcto\n";
			break;
		case -5:
			ret = "Tipo de usuario ingresado en los permisos por caracter invalido\n";
			break;
		case -6:
			ret = "Tipo de usuario invalido\n";
			break;
		case -7:
			ret = "El tipo de usuario NO es acompañado por el literl '=' \n";
			break;
		case -8:
			ret = "Se ha encontrado un tipo de permiso repetido en al secuencia, esto NO esta permitido\n";
	}

	return ret;
}

/*
 * Método para determinar que un número pertenezca a los valores permitidos en Octal de chmod
*/
int PerteneceNums(char n){
	int i;
	int a = (int)n; 
	int ret = 0;
	for(i = 0; i < NELEMS(ALLOWED_OCTAL) && ret == 0; i++){
		if(a == (int)(ALLOWED_OCTAL[i])){
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
	if (sizeof(Perms) > 3)
		ret = ERR_EXTRA_PARAMS; //Código de error propio, como hay más parámetros de los permitidos 
								//(OJO, NO CONSIDERO STICKY BIT, PREGUNTAR), corto y guardo esto como código de error 
	else{
		if(sizeof(Perms) < 3)
			ret = ERR_INSUFFICIENT_PARAMS; //Código de error propio "Cantidad de permisos a setear insuficiente, debe ser del tipo ugo"
		else{
			for(i = 1; i < sizeof(Perms) && ret == 1; i++){
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
		if(c == ALLOWED_PERMS[i])
			ret = 1; 
	}
	if(ret != 1) //si NO encontré el literal
		ret = ERR_INCORRECT_PERM; //Codigo de error propio "Permiso en caracteres incorrecto"

	return ret;
}


/*
 * Método con el cual intento corroborar que la llamada POR STRING de chmod sea correcta 
*/
int LeerChars(char *Perms){
	int ret = 1;
	int i = 0;
	char comp = 'a';
	char lect = '';
	if(Perms[0] == comp){
		if(Perms[1] == '=' && ret == 1){
			for(i = 2; i < sizeof(Perms) && ret == 1; i++){
				if(comp != Perms[i]){ //Pregunto que NO se me estén repitiendo permisos (chmod no permite a=wrr, por ejemplo), la primera vez debe entrar
					ret = perteneceChar(Perms[i]); //de ahi en más, se que los permisos son del tipo a=<algo>, tengo que corroborar que cada caracter de <algo> sea legal
					comp = Perms[i]; //guardo el caracter usado para ver luego que no se me repita
				}
				else
					ret = ERR_REPEAT_PERM; //Si NO entré al if, es porque hay permisos repetidos, es un error
			}
		}
		else{
			ret = ERR_WRONG_LITERAL; //Error: "El tipo de usuario NO es acompañado por el caracter "=" "
		}
	}
	else{ //EL caracteter de usuario NO es 'a', debe ser 'u' inicialmente (u=...,g=...,o=...)
		if(Perms[0] == 'u'){
			comp = 'u';
			while(i < sizeof(Perms) && ret == 1){ //Mientras haya algo que leer en el string de permisos
				while (lect != ',' && ret == 1){ //Mientras NO este pasando de grupo de usuarios

					lect = Perms[i];
					if(comp != Perms[i]){ //Pregunto que NO se me estén repitiendo permisos (chmod no permite u=wrr, por ejemplo), la primera vez debe entrar
						ret = perteneceChar(Perms[i]); 
						comp = Perms[i]; //guardo el caracter usado para ver luego que no se me repita
					}
					else{
						ret = ERR_REPEAT_PERM; //Se repitió un permiso y ademas estoy en un ciclo interno, tengo que salir de aquí!
						break;
					}
					i++;
				}
				i++; //Estoy en una ',', hago un paso hacia adelante, hacia el siguiente grupo
				comp = ALLOWED_GROUPS[i]; //terminé de verificar los permisos para un grupo, paso al siguiente
				if(comp != Perms[i]) //Verifico que el siguiente tipo de usuario sea válido (debe ser 'g' u 'o')
					ret = ERR_INVALID_USER_TYPE; //Error: "tipo de usuario inválido"
				else{
					i++;
					if(Perms[i] != '=')
						ret = ERR_WRONG_LITERAL;
				}
			}
		}
		else{
			ret = ERR_WRONG_USER; //codigo de error propio: "Usuario Incorrecto para los permisos"
		}		
	}
	return ret;
}


int main(int argc, const char * argv[]){

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
				errno = LeerOctal(Permisos); //Me estan intentando ejecutar chmod en Octal, chequear sintaxis
		else
			if(perteneceChar(Permisos[0]))
				errno = LeerChars(Permisos); //Me estan intentando ejecutar chmod en modo texto, chequear sintaxis
			else{ //Si no entra por ningún lado, me mandaron cualquier cosa, abortar y reportar
				fprintf(stderr, "La llamada que se esta intentando hacer NO coincide con chmod");
				exit(1);
			}

		if(errno == 1){
			//Si llegué hasta aquí, el control fue correcto, puedo invocar a chmod
			errno = chmod (FILE,Permisos); 
			 if (errno < 0){ //
	        	fprintf(stderr, "%s: error en la llamada chmod(%s, %s) - %d (%s)\n",
	                	argv[0], buf, mode, errno, strerror(errno));
	        	exit(1);
	    	}
	    	return(0);
    	}
    	else{
    		fprintf(stderr, "error en el analisis de los datos suministrados con codigo %d, lo que significa: %s \n", errno, MensError(errno));
    		exit(1);
    	}
	}
}