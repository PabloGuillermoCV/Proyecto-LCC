;---------------------------------------------------------------------------
;EJERCICIO 1
;---------------------------------------------------------------------------

(defun rango (M N)
    (cond
        (
			(< M N) ;Si M es menor a N
				(write M)
				(write-char #\Space)
				(rango (+ M 1) N)
        )
        (
			(= M N) ;Si M y N son iguales
				(write N)
        )
        (
            (> M N) ;Si M es mayor a N
                (write "ERROR: El primer elemento debe ser menor o igual al segundo.")
        )
        (
            T
                ()
        )
    )
)
(rango 2 6)

;---------------------------------------------------------------------------
;EJERCICIO 2
;---------------------------------------------------------------------------

(defun ordenarl (L)
    (setq Menor (car L))
    (cond
        (
            (null L) ;Si L esta vacia
                ()
        )
        (
			T ;Caso contrario
				(loop for X in L do ;Para cada elemento (lista) X de la lista L
					(cond
						(
							(> (list-length Menor) (list-length X))
								(setq Menor X)
						)
						(
							(< (list-length Menor) (list-length X))
								()
						)
						(
							(= (list-length Menor) (list-length X))
								()
						)
						(
							T
								()
						)
					)
				)
				(write Menor)
				(ordenarl (remove Menor L))
        )
    )
)
(ordenarl '((a b c) (d e) (f g h) (i j k l) (m)))

;---------------------------------------------------------------------------
;EJERCICIO 3
;---------------------------------------------------------------------------

;recordar que NIL = [] en este caso ------> car o cdr con nil retorna []
(defun partes (L)
    (cond 
        (
            (null L) 
                (car nil)
        )
        (
			T
				(setq PX (partes (cdr L)))
				(setq Comb (combinar (car L) PX))
				(append Comb PX)
        )
    )
)

;hay que verificar que X sea un átomo y no otra cosa, ATOM verifica esto
;Otro drama, parece que APPEND no une elementos atómicos y listas, habria que ver esto, se me ocurre crear listas de un elemento para 
;saltar esta limitación, pero hay que probarlo
;dudo si hacer cosas como '((X)) es correcto para decir "La lista que contiene a X como elemento"
(defun combinar (X L)
    (cond
        ;Tengo duda de esto, estás preguntando si x es un atomo pero despues
        ;no hacés nada. Lo de null L forma parte de otra condicion distinta.
        ;Lo dejo así como está porque no se cual de las 2 condiciones seria.
        (atom X)
        (
            (null L) 
                (list NIL)
        )
        (
			T
				(setq Rta (append '(car L) '(combinar (X (cdr L)))))
				;No creo que sea necesario poner parentesis dobles
				(append '(X) Rta)
        )
    )
)
(partes '(a b))