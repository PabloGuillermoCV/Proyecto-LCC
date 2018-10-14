#include<stdlib.h>

//(AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE (AoBoC)(AoBoC)DE...

/*
	
	sem(ABC) = 1
		A hace: down(ABC), Imprimir, up(D)
		B hace: down(ABC), Imprimir, up(D)
		C hace: down(ABC), Imprimir, up(D) 
	sem(D) = 0
		D hace: down(D), up(ABC), down(D), Imprimir, up(E)
	sem(E) = 0
		E hace: down(E), Imprimir, up(ABC)
	
*/