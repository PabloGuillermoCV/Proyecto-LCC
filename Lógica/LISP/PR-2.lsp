;---------------------------------------------------------------------------
;EJERCICIO 1
;---------------------------------------------------------------------------

(defun rango (M N)
    (cond
        (
            (and (integerp M) (integerp N)) ;Si M y N son enteros
                (cond
                    (
                        (< M N) ;Si M es menor a N
                            (cons M (rango (+ M 1) N))
                    )
                    (
                        (= M N) ;Si M y N son iguales
                            (cons N nil)
                    )
                    (
                        (> M N) ;Si M es mayor a N
                            "ERROR: El primer elemento debe ser menor o igual al segundo."
                    )
                    (
                        T
                            ()
                    )
                )
        )
        (
            T ;Si M y/o N no son enteros
                "ERROR: Los valores ingresados no son ambos numeros enteros."
        )
    )
)
(write (rango 2 6))

;---------------------------------------------------------------------------
;EJERCICIO 2
;---------------------------------------------------------------------------

(defun ordenarl (L)
    (cond
        (
            (listp L)
                (cond
                    (
                        (null L) ;Si L esta vacia
                            ()
                    )
                    (
                        T ;Caso contrario
                            (
                                let ((Menor (sacarMenor L)))
                                    (cons Menor (ordenarL (remove Menor L)))
                            )
                    )
                )
        )
        (
            T
                "ERROR: El valor ingresado no es una lista"
        )
    )
)

(defun sacarMenor (L) ;Obtiene la menor sublista de L
    (cond
        (
            (= (list-length L) 0) ;Si L esta vacia
                ()
        )
        (
            (= (list-length L) 1) ;Si L tiene 1 solo elemento
                (car L)
        )
        (
            T ;Si L tiene mas de 1 elemento
                (
                    let ((MenorSubLista (sacarMenor (cdr L))))
                        (cond
                            (
                                (> (list-length (car L)) (list-length MenorSubLista))
                                    MenorSubLista
                            )
                            (
                                T
                                    (car L)
                            )
                        )
                )
        )
    )
)
(write (ordenarl '((a b c) (d e) (f g h) (i j k l) (m))))

;---------------------------------------------------------------------------
;EJERCICIO 3
;---------------------------------------------------------------------------

(defun partes (S)
    (cond
        (
            (listp S)
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
        (
            T
                "ERROR: El valor ingresado no es una lista"
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
                ;Combinar es la lista S con y sin X, y el combinar del resto de S
                (cons (car S) (cons (cons X (car S)) (combinar X (cdr S))))
        )
    )
)
(write (partes '(1 2 3)))