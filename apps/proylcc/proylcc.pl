:- module(proylcc, 
	[  
		grid/2,
		flick/3
	]).

grid(1, [
		 [y,g,b,g,v,y,p,v,b,p,v,p,v,r],
		 [r,r,p,p,g,v,v,r,r,b,g,v,p,r],
		 [b,v,g,y,b,g,r,g,p,g,p,r,y,y],
		 [r,p,y,y,y,p,y,g,r,g,y,v,y,p],
		 [y,p,y,v,y,g,g,v,r,b,v,y,r,g],
		 [r,b,v,g,b,r,y,p,b,p,y,r,y,y],
		 [p,g,v,y,y,r,b,r,v,r,v,y,p,y],
		 [b,y,v,g,r,v,r,g,b,y,b,y,p,g],
		 [r,b,b,v,g,v,p,y,r,v,r,y,p,g],
		 [v,b,g,v,v,r,g,y,b,b,b,b,r,y],
		 [v,v,b,r,p,b,g,g,p,p,b,y,v,p],
		 [r,p,g,y,v,y,r,b,v,r,b,y,r,v],
		 [r,b,b,v,p,y,p,r,b,g,p,y,b,r],
		 [v,g,p,b,v,v,g,g,g,b,v,g,g,g]
		 ]).

grid(2, [
		 [y,y,b,g,v,y,p,v,b,p,v,p,v,r],
		 [y,y,p,p,g,v,v,r,r,b,g,v,p,r],
		 [b,y,g,y,b,g,r,g,p,g,p,r,y,y],
		 [r,y,y,y,y,p,y,g,r,g,y,v,y,p],
		 [y,p,y,v,y,g,g,v,r,b,v,y,r,g],
		 [r,b,v,g,b,r,y,p,b,p,y,r,y,y],
		 [p,g,v,y,y,r,b,r,v,r,v,y,p,y],
		 [b,y,v,g,r,v,r,g,b,y,b,y,p,g],
		 [r,b,b,v,g,v,p,y,r,v,r,y,p,g],
		 [v,b,g,v,v,r,g,y,b,b,b,b,r,y],
		 [v,v,b,r,p,b,g,g,p,p,b,y,v,p],
		 [r,p,g,y,v,y,r,b,v,r,b,y,r,v],
		 [r,b,b,v,p,y,p,r,b,g,p,y,b,r],
		 [v,g,p,b,v,v,g,g,g,b,v,g,g,g]
		 ]).
		 
grid(3, [
		 [y,b,g,v,p,p,v,r,r,v,b,y,y,g],
		 [r,b,r,r,v,y,g,r,p,p,r,y,g,v],
		 [r,y,g,b,b,r,v,v,y,v,y,y,r,b],
		 [r,r,g,v,b,v,b,r,v,y,y,g,g,g],
		 [r,v,p,p,r,r,v,g,r,g,g,v,r,p],
		 [v,r,v,y,y,r,p,g,p,r,g,p,y,g],
		 [r,g,v,r,p,g,y,p,p,g,g,v,r,r],
		 [g,v,b,y,g,g,v,y,p,b,r,p,p,b],
		 [y,p,g,v,b,y,r,p,g,r,v,p,r,g],
		 [r,g,p,y,v,g,p,r,r,y,g,v,g,r],
		 [b,b,r,p,g,v,r,y,g,v,p,g,g,b],
		 [r,y,v,g,p,r,g,v,b,b,g,b,r,p],
		 [y,y,y,v,p,r,v,g,y,g,v,b,r,r],
		 [r,g,b,v,g,r,g,g,y,y,v,y,p,y]
		 ]).
		 
grid(4, [
		 [y,g,v,r,g,y,b,y,g,b,y,r,g,v],
		 [g,y,v,g,y,r,v,b,p,p,g,g,r,y],
		 [g,g,v,p,r,r,y,y,p,g,b,v,g,b],
		 [y,r,p,v,g,b,y,r,p,b,g,r,v,b],
		 [r,p,b,v,p,g,r,p,g,r,y,v,p,v],
		 [r,p,v,g,r,y,y,v,b,y,b,b,p,b],
		 [g,r,y,v,p,g,y,b,y,v,y,r,g,p],
		 [b,g,r,p,g,b,g,v,b,g,p,r,g,b],
		 [b,g,r,p,g,y,y,g,b,y,y,y,g,v],
		 [v,v,v,r,y,g,b,p,y,v,b,g,r,g],
		 [y,r,g,b,v,p,g,g,r,p,v,b,p,r],
		 [r,g,p,g,b,v,b,g,p,r,y,g,p,g],
		 [g,y,v,b,r,p,p,g,p,r,b,r,g,g],
		 [r,g,p,b,r,y,g,b,v,g,p,v,r,b]
		 ]).
		 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% flick(+Grid, +Color, -RGrid)
%
% RGrid es el resultado de hacer 'flick' de la grilla Grid con el color Color. 

flick(Grid,Color,RGrid):- Grid=[[Columna1|Columnas]|Filas], verificarColor(0,0,Columna1,Color,Grid,RGrid).
%flick(Grid,Color,RGrid):- pintarCeldaVer(5,6,Color,Grid,RGrid). PRUEBA DE QUE PINTAR UNA CELDA CUALQUIERA FUNCIONA


%buscarCeldaVer(+X,+Y,+Color,+Grilla,-Res)
%Busco para ver si la celda coincide con el color que pido.

buscarCeldaVer(X,0,C,[Fila1|_Filas],R):- buscarCeldaHor(X,C,Fila1,R). %Encontré la fila en Y que buscaba, empiezo a recorrer sus columnas a derecha.
buscarCeldaVer(X,Y,C,[_Fila1|Filas],R):- Y>0, YAux is Y-1, buscarCeldaVer(X,YAux,C,Filas,R).


%buscarCeldaHor(+X,+Color,+Grilla,-Res)
%Busco para ver si la celda coincide con el color que pido.

buscarCeldaHor(0,C,[Columna1|_Columnas],Columna1):- C=Columna1. %Llegué a la posición X,Y que buscaba y coincide el color.
buscarCeldaHor(X,C,[_Columna1|Columnas],R):- X>0, XAux is X-1, buscarCeldaHor(XAux,C,Columnas,R).


%pintarCeldaVer(+X,+Y,+ColorNew,+Grilla,+NewGrilla)
%Se cual es el color de la celda que busco y la pinto.

pintarCeldaVer(X,0,C,[Fila1|Filas],[R|Filas]):- pintarCeldaHor(X,C,Fila1,R).
pintarCeldaVer(X,Y,C,[Fila1|Filas],[Fila1|R]):- Y>0, YAux is Y-1, pintarCeldaVer(X,YAux,C,Filas,R).


%pintarCeldaHor(+X,+ColorNew,+Grilla,+NewGrilla)
%Se cual es el color de la celda que busco y la pinto.

pintarCeldaHor(0,C,[_Columna1|Columnas],[C|Columnas]).
pintarCeldaHor(X,C,[Columna1|Columnas],[Columna1|R]):- X>0, XAux is X-1, pintarCeldaHor(XAux,C,Columnas,R).


%verificarColor(+X,+Y,+ColorActual,+ColorNew,+Grilla,-NewGrilla)
%Comienzo el proceso de ver cada celda adyacente.

verificarColor(X,Y,ColorActual,ColorNew,Grilla,Grilla):- X<0. %Me cai de la grilla y me detengo.
verificarColor(X,Y,ColorActual,ColorNew,Grilla,Grilla):- Y<0. %Me cai de la grilla y me detengo.
verificarColor(X,Y,ColorActual,ColorNew,Grilla,Grilla):- buscarCeldaVer(X,Y,ColorActual,Grilla,F), ColorActual\=F. %Los colores no coinciden, me detengo.
verificarColor(X,Y,ColorActual,ColorNew,Grilla,NewGrilla):- buscarCeldaVer(X,Y,ColorActual,Grilla,F), 
															ColorActual=F, 
															pintarCeldaVer(X,Y,ColorNew,Grilla,GrillaAux), 
															buscarAdyacencia(X,Y,ColorAtcual,ColorNew,GrillaAux,NewGrilla).


%buscarAdyacencia(+X,+Y,+ColorAtcual,+ColorNew,+Grilla,-NewGrilla)
%Busco en las cuatro celdas adyacentes a X e Y y repito el proceso de verificar el color.
%Si el color no coincide, ese verificar retorna la misma grilla (lo cual ocurrirá por lo menos 1 vez de las 4).

buscarAdyacencia(X,Y,ColorActual,ColorNew,Grilla,NewGrilla):- XP is X+1, YP is Y+1, XN is X-1, YN is Y-1, 
															  verificarColor(XP,Y,ColorActual,ColorNew,Grilla,Grilla1), 
															  verificarColor(X,YP,ColorActual,ColorNew,Grilla,Grilla2), 
															  verificarColor(XN,Y,ColorActual,ColorNew,Grilla2,Grilla3), 
															  verificarColor(X,YN,ColorActual,ColorNew,Grilla3,NewGrilla).

