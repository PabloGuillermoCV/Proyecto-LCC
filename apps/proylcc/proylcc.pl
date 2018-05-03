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

flick(Grid,Color,RGrid):- Grid=[[Columna1|_Columnas]|_Filas], verificarColor(0,0,Columna1,Color,Grid,RGrid).
%En este caso, Columna1 es el color ubicado arriba a la izquierda (en 0,0) de la grilla Grid.

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

ayudaBasica(Grid,Color,Res):- Grid=[[Columna1|_Columnas]|_Filas], marcarCeldas(0,0,Columna1,Color,Grid,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%marcarCeldas(+X,+Y,+ColorActual,+ColorNew,+Grilla,-Res)
%Llama a verificarColor para que pinte toda la grilla (usando verificarColor) del color con el que se selecciono la ayuda (esto no afecta a la grilla en pantalla).
%Luego con esa grilla resultante pasa a llamar a contarCeldas para que cuente todas las celdas del nuevo color adyacentes entre si y retorne el resultado.

marcarCeldas(X,Y,ColorActual,ColorNew,Grilla,Res):- verificarColor(X,Y,ColorActual,ColorNew,Grilla,GrillaPintada), contarCeldas(X,Y,ColorNew,GrillaPintada,Res).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%contarCeldas(+X,+Y,+Color,+Grilla,-GrillaNew,-Res)
%Se encarga de recorrer la grilla contando cada uno de las celdas con color igual a Color y sumando a un contador.
%Se pinta una grilla auxiliar para evitar ciclar por lugares que ya se contaron.
%El proceso de recursion es el mismo que en verificarColor.

contarCeldas(X,_Y,_Color,_Grilla,_GrillaNew,0):- X < 0. %Me cai de la grilla y me detengo.
contarCeldas(_X,Y,_Color,_Grilla,_GrillaNew,0):- Y < 0. %Me cai de la grilla y me detengo.
contarCeldas(X,_Y,_Color,_Grilla,_GrillaNew,0):- X > 13. %Me cai de la grilla y me detengo.
contarCeldas(_X,Y,_Color,_Grilla,_GrillaNew,0):- Y > 13. %Me cai de la grilla y me detengo.
contarCeldas(X,Y,Color,Grilla,_GrillaNew,0):- buscarCeldaVer(X,Y,Grilla,F), F \= Color. %Los colores no coinciden, me detengo.
contarCeldas(X,Y,Color,Grilla,GrillaNew,Res):- buscarCeldaVer(X,Y,Grilla,F), 
											   F = Color, 
											   pintarCeldaVer(X,Y,'n',Grilla,GrillaAux0), 
											   XP is X+1, YP is Y+1, XN is X-1, YN is Y-1, 
											   contarCeldas(XP,Y,Color,GrillaAux0,GrillaAux1,Res1), 
											   contarCeldas(X,YP,Color,GrillaAux1,GrillaAux2,Res2), 
											   contarCeldas(XN,Y,Color,GrillaAux2,GrillaAux3,Res3), 
											   contarCeldas(X,YN,Color,GrillaAux3,GrillaNew,Res4),
											   Res12 is Res1+Res2, Res123 is Res12+Res3, Res1234 is Res123+Res4, Res is Res1234+1.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%IMPORTANTE: Honestamente no entiendo que es lo que me pide la segunda ayuda en el enunciado del proyecto.

%ayudaExtendidaShell(+Grid,+Color,-Res)
%Llamo al ayudaExtendida con cada color y luego con la grilla que me retorna realizo lo mismo que en ayudaBasica para obtener un valor numerico con cada combinacion.

ayudaExtendidaShell(Grid,Color,Res):- ayudaExtendida(Grid,'y',GridY), ayudaBasica(GridY,Color,ResY), 
									  ayudaExtendida(Grid,'r',GridR), ayudaBasica(GridR,Color,ResR), 
									  ayudaExtendida(Grid,'g',GridG), ayudaBasica(GridG,Color,ResG), 
									  ayudaExtendida(Grid,'p',GridP), ayudaBasica(GridP,Color,ResP), 
									  ayudaExtendida(Grid,'b',GridB), ayudaBasica(GridB,Color,ResB), 
									  ayudaExtendida(Grid,'v',GridV), ayudaBasica(GridV,Color,ResV), 
									  Res is ResY + ResR + ResG + ResP + ResB + ResV.

%ayudaExtendida(+Grid,+Color,-NewGrid)
%Similar al ayudaBasica pero este solamente retorna la grilla pintada de un color y no calcula ningun valor numerico

ayudaExtendida(Grid,Color,NewGrid):- Grid=[[Columna1|_Columnas]|_Filas], verificarColor(0,0,Columna1,Color,Grid,NewGrid).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%IMPORTANTE: Estoy pensando que no sera necesario hacer un proceso para cambiar la grilla celda por celda. Se podria simplemente retornar la siguiente?
%Dejo el codigo que empeze por las dudas.

%cambiarGrilla(+GridOrigen,+GridNueva,-GridResultado)
%Dada la grilla actual origen sin modificar y la grilla siguiente en la lista, se reemplaza cada elemento de Origen por Nueva para formar la grilla Resultado.

%cambiarGrilla(GridOrigen,GridNueva,GridResultado):- cambiarGrillaV(0,GridOrigen,GridNueva,GridResultado).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%cambiarGrillaV(+Y,+GridOrigen,+GridNueva,-GridResultado)

%cambiarGrillaV(13,GO,GN,GR):- .
%cambiarGrillaV(Y,GO,GN,GR):- GO=[[GOColumna1|GOColumnas]|GOFilas], 
%							   GN=[[GNColumna1|GNColumnas]|GNFilas], 
%							   cambiarGrillaH(0,GOColumnas,GNColumnas,GRColumnas), 
%							   YAux is Y+1, 
%							   cambiarGrilla(X,YAux,GOFilas,GNFilas,GRFilas), 
%							   GR = [[GNColumna1|GRColumnas]|GRFilas].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%