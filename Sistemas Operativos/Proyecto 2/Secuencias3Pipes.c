#include<stdlib.h>

//ACDE BCDE ABCDE  ACDE BCDE ABCDE...

/*
	
	sem(A) = 1
		A hace: down(A), Imprimir, up(C), down(A), Imprimir, up(B)
	sem(B) = 0
		B hace: down(B), Imprimir, up(C)
	sem(C) = 0
		C hace: down(C), Imprimir, up(D) 
	sem(D) = 0
		D hace: down(D), Imprimir, up(E)
	sem(E) = 0
		E hace: down(E), Imprimir, up(B), down(E), Imprimir, up(A), down(E), Imprimir, up(A)
	
*/