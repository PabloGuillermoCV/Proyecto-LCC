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
		 [y,b,r,r,v,y,p,v,b,p,v,p,v,y],
		 [b,v,p,p,g,v,v,r,r,b,g,v,p,p],
		 [b,v,g,y,v,b,r,v,r,r,p,r,y,y],
		 [r,y,r,r,g,r,y,b,v,g,y,v,y,p],
		 [y,p,y,b,y,y,g,v,g,b,p,y,r,b],
		 [b,g,y,r,b,r,y,p,b,p,y,r,v,r],
		 [r,y,b,p,y,r,b,v,r,r,v,b,v,r],
		 [r,y,g,g,r,r,p,g,b,p,b,b,p,g],
		 [v,b,r,v,g,g,r,y,r,v,g,g,r,y],
		 [y,b,g,v,v,r,r,y,b,b,g,b,g,y],
		 [y,v,b,p,p,b,b,y,p,p,b,v,g,p],
		 [b,p,g,y,v,y,b,y,v,r,b,y,r,v],
		 [g,b,b,v,p,y,p,r,b,g,p,y,b,r],
		 [y,r,y,r,b,v,r,r,v,b,g,y,g,y]
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

%flick(+Grid,+Color,-RGrid)
%RGrid es el resultado de hacer 'flick' de la grilla Grid con el color Color. 
%En este caso, el verificarColor, comenzara a revisar la grilla celda por celda comenzando por arriba a la izquierda (en la celda 0,0).
%Si la primera posicion de la grilla es igual a la de Color, entonces se retorna la grilla normal sin cambiar nada.

flick(Grid,Color,Grid):- Grid=[[Columna1|_Columnas]|_Filas], Color = Columna1.
flick(Grid,Color,RGrid):- Grid=[[Columna1|_Columnas]|_Filas], verificarColor(0,0,Columna1,Color,Grid,RGrid).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%buscarCeldaVer(+X,+Y,+Grilla,-Res)
%Recorro las filas (el eje Y) para buscar una celda especifica.

buscarCeldaVer(X,0,[Fila1|_Filas],R):- buscarCeldaHor(X,Fila1,R). %Encontré la fila en Y que buscaba, empiezo a recorrer sus columnas a derecha.
buscarCeldaVer(X,Y,[_Fila1|Filas],R):- Y > 0, YAux is Y-1, buscarCeldaVer(X,YAux,Filas,R).

%buscarCeldaHor(+X,+Grilla,-Res)
%Recorro las columnas (el eje X) para buscar una celda especifica.

buscarCeldaHor(0,[Columna1|_Columnas],Columna1). %Llegué a la posición X,Y que buscaba.
buscarCeldaHor(X,[_Columna1|Columnas],R):- X > 0, XAux is X-1, buscarCeldaHor(XAux,Columnas,R).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%pintarCeldaVer(+X,+Y,+ColorNew,+Grilla,+NewGrilla)
%Recorro las filas (el eje Y) para pintar una celda especifica.

pintarCeldaVer(X,0,C,[Fila1|Filas],[R|Filas]):- pintarCeldaHor(X,C,Fila1,R). %Encontré la fila en Y que buscaba, empiezo a recorrer sus columnas a derecha.
pintarCeldaVer(X,Y,C,[Fila1|Filas],[Fila1|R]):- Y > 0, YAux is Y-1, pintarCeldaVer(X,YAux,C,Filas,R).

%pintarCeldaHor(+X,+ColorNew,+Grilla,+NewGrilla)
%Recorro las columnas (el eje X) para pintar una celda especifica.

pintarCeldaHor(0,C,[_Columna1|Columnas],[C|Columnas]). %Llegué a la posición X,Y que buscaba, la retorno con un color distinto.
pintarCeldaHor(X,C,[Columna1|Columnas],[Columna1|R]):- X > 0, XAux is X-1, pintarCeldaHor(XAux,C,Columnas,R).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%verificarColor(+X,+Y,+ColorActual,+ColorNew,+Grilla,-NewGrilla)
%Comienza buscando el color de la posicion actual y si coincide con el actual, en caso de que no coincida (o me caiga de la grilla) retorno la grilla sin tocarla.
%Si coincide el color, entonces la pinto del color nuevo.
%A continuacion repito el proceso para las cuatro posiciones adyacentes a la posicion actual.
%Esto se repite hasta que no sea posible pintar mas celdas.

verificarColor(X,_Y,_ColorActual,_ColorNew,Grilla,Grilla):- X < 0. %Me cai de la grilla y me detengo.
verificarColor(_X,Y,_ColorActual,_ColorNew,Grilla,Grilla):- Y < 0. %Me cai de la grilla y me detengo.
verificarColor(X,_Y,_ColorActual,_ColorNew,Grilla,Grilla):- X > 13. %Me cai de la grilla y me detengo.
verificarColor(_X,Y,_ColorActual,_ColorNew,Grilla,Grilla):- Y > 13. %Me cai de la grilla y me detengo.
verificarColor(X,Y,ColorActual,_ColorNew,Grilla,Grilla):- buscarCeldaVer(X,Y,Grilla,F), F \= ColorActual. %Los colores no coinciden, me detengo.
verificarColor(X,Y,ColorActual,ColorNew,Grilla,NewGrilla):- buscarCeldaVer(X,Y,Grilla,F), 
															F = ColorActual, 
															pintarCeldaVer(X,Y,ColorNew,Grilla,GrillaAux0), 
															XP is X+1, YP is Y+1, XN is X-1, YN is Y-1, 
															verificarColor(XP,Y,ColorActual,ColorNew,GrillaAux0,GrillaAux1), 
															verificarColor(X,YP,ColorActual,ColorNew,GrillaAux1,GrillaAux2), 
															verificarColor(XN,Y,ColorActual,ColorNew,GrillaAux2,GrillaAux3), 
															verificarColor(X,YN,ColorActual,ColorNew,GrillaAux3,NewGrilla).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%ayudaBasica(+Grid,+Color,-Res)
%Depende de cual de los 5 colores precise, llamara a un metodo que contara todas las celdas pintadas al final con dicho color.
%Es similar al flick pero este no cambia la grilla, solo retorna un valor numerico.
%Esto sera llamado externamente 6 veces para una ayuda de cada color.
%Llama a flick para que pinte toda la grilla (usando verificarColor) del color con el que se selecciono la ayuda (esto no afecta a la grilla en pantalla).
%Luego se vuelve a pintar la grilla con un valor 'n' (esto no afecta a la grilla en pantalla).
%Luego con esa grilla resultante pasa a llamar a contarCeldasFil para que cuente todas las celdas que tengan ese valor 'n'.

ayudaBasicaShell(Grid,ResR,ResV,ResP,ResG,ResB,ResY):- ayudaBasica(Grid,"r",ResR), ayudaBasica(Grid,"v",ResV), ayudaBasica(Grid,"p",ResP), 
													   ayudaBasica(Grid,"g",ResG), ayudaBasica(Grid,"b",ResB), ayudaBasica(Grid,"y",ResY).

ayudaBasica(Grid,Color,Res):- flick(Grid,Color,GrillaPintada), flick(GrillaPintada,"n",GrillaPintadaN), contarCeldasFil("n",GrillaPintadaN,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%contarCeldas(+X,+Y,+Color,+Grilla,-Res)
%Se encarga de recorrer la grilla contando cada uno de las celdas con color igual a Color y sumando a un contador.

contarCeldasFil(_Color,[],0).
contarCeldasFil(Color,[Fila1|Filas],Res):- contarCeldasCol(Color,Fila1,ResCol), contarCeldasFil(Color,Filas,ResFil), Res is ResCol + ResFil.

contarCeldasCol(_Color,[],0).
contarCeldasCol(Color,[Columna1|Columnas],Res):- Color = Columna1, contarCeldasCol(Color,Columnas,ResAux), Res is ResAux + 1.
contarCeldasCol(Color,[Columna1|Columnas],Res):- Color \= Columna1, contarCeldasCol(Color,Columnas,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%ayudaExtendida(+Grid,+Color,-Res)
%Pinto a la grilla de un color dado, y con ese resultado comienzo a usar ayudaBasica para ver las posibilidades de cada color y poder elegir la mayor.
%Su funcionamiento es similar al ayudaBasica

ayudaExtendidaShell(Grid,ResR,ResV,ResP,ResG,ResB,ResY):- ayudaExtendida(Grid,"r",ResR), ayudaExtendida(Grid,"v",ResV), ayudaExtendida(Grid,"p",ResP), 
														  ayudaExtendida(Grid,"g",ResG), ayudaExtendida(Grid,"b",ResB), ayudaExtendida(Grid,"y",ResY).

ayudaExtendida(Grid,Color,Res):- flick(Grid,Color,NewGrid), 
								 ayudaBasicaShell(NewGrid,ResR,ResV,ResP,ResG,ResB,ResY), 
								 L1 = [ResV|ResR], 
								 L2 = [ResP|L1], 
								 L3 = [ResG|L2], 
								 L4 = [ResB|L3], 
								 calcularMayor(L4,ResY,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%calcularMayor(+L,+M,-Res)
%Predicado auxiliar que se encarga de calcular el mayor elemento de una lista.

calcularMayor([],M,M).
calcularMayor([X|Xs],M,Res):- X =< M, calcularMayor(Xs,M,Res).
calcularMayor([X|Xs],M,Res):- X > M, calcularMayor(Xs,X,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%