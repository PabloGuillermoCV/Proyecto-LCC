
/*
	todo este codigo queda de back up sobre los ejercicios mios por si despues de probar que lo que tenemos
	ahora anda, lo podemos agregar y hacer los ejercicios de forma más completa 
*/

#define ERR_INCORRECT_PERM -4
#define ERR_WRONG_USER -5
#define ERR_INVALID_USER_TYPE -6
#define ERR_WRONG_LITERAL -7
#define ERR_REPEAT_PERM -8

/*case -4:
			ret = "El caracter de permisos ingresado NO es correcto\n";
			break;
		case -5:
			ret = "Tipo de usuario ingresado en los permisos por caracter invalido\n";
			break;
		case -6:
			ret = "Tipo de usuario invalido\n";
			break;
		case -7:
			ret = "El tipo de usuario NO es acompañado por el literal '=' \n";
			break;
		case -8:
			ret = "Se ha encontrado un tipo de permiso repetido en al secuencia, esto NO esta permitido\n";*/

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
	int p = 0;
	char comp = 'a';
	char lect = '';
	if(Perms[0] == comp){ //Primer caso, se esta intentando setear los permisos de fora global usando "a=..."
		if(Perms[1] == '=' && ret == 1){ //Si es asi, pregunto si hay un '=' acompañando a la "a", la parte de ret == 1 es por si al entrar corté el ciclo por un error 
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
				p++; //Uso el contador exclusivo de los permisos para pasar al siguiente grupo
				comp = ALLOWED_GROUPS[p]; //terminé de verificar los permisos para un grupo, paso al siguiente
				if(comp != Perms[i]) //Verifico que el siguiente tipo de usuario sea válido (debe ser 'g' u 'o')
					ret = ERR_INVALID_USER_TYPE; //Error: "tipo de usuario inválido"
				else{
					i++;
					if(Perms[i] != '='){
						ret = ERR_WRONG_LITERAL;
						i++; //Me muevo un lugar más, hacia el siguiente permiso para el nuevo grupo
					}
				}
			}
		}
		else{
			ret = ERR_WRONG_USER; //codigo de error propio: "Usuario Incorrecto para los permisos"
		}		
	}
	return ret;
}
