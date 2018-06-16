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

(defun partes (S)
    (cond
        (
            (null S) ;Si S esta vacia
                '(()) ;No hay nada que poner, retorno una lista vacia.
        )
        (
            T ;Caso contrario
                (combinar (car S) (partes (cdr S)))
        )
    )
)

(defun combinar (X S)
    (cond
        (
            (null S) ;Si S esta vacia
                () ;No agrego nada a ninguna lista.
        )
        (
            T ;Caso contrario
                (cons (car S) (cons (cons X (car S)) (combinar X (cdr S))))
        )
    )
)
(write (partes '(1 2 3)))


;--------
;Resolucion alternativa en caso de que la otra no sea valida

;recordar que NIL = [] en este caso ------> car o cdr con nil retorna []
(defun partes (S)
    (cond 
        (
            (null S) 
                (car nil)
        )
        (
			T
				(setq PX (partes (cdr S)))
				(setq Comb (combinar (car S) PX))
				(append Comb PX)
        )
    )
)

;hay que verificar que X sea un átomo y no otra cosa, ATOM verifica esto
;Otro drama, parece que APPEND no une elementos atómicos y listas, habria que ver esto, se me ocurre crear listas de un elemento para 
;saltar esta limitación, pero hay que probarlo
;dudo si hacer cosas como '((X)) es correcto para decir "La lista que contiene a X como elemento"
(defun combinar (X S)
    (cond
        (
            (atom X) 
                (cond
                    (
                        (null S) 
                            (list NIL)
                    )
                    (
                		T
                			(setq Rta (append '(car S) '(combinar (X (cdr S)))))
                			(append '(X) Rta)
                    )
                )
        )
        (
            T
                ()
        )
    )
)
(partes '(a b))