#include<stdlib.h>

/*
	sem(A) = 1
		A hace:  Imprimir, down(A), up(B)
	sem(B) = 0
		B hace: down(B), Imprimir, up(C)
	sem(C) = 0
		C hace: down(C), Imprimir, up(D) 
	sem(D) = 0
		D hace: down(D), Imprimir, up(E)
	sem(E) = 0
		E hace: down(E), Imprimir, up(A)

	

*/