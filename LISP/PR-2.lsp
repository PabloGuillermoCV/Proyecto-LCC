;se usa Punto y coma para los comentarios, por lo que veo
;OJO con SETQ, Mauro habia dicho de usar LET en vez SETQ
(defun rango (M N)
    (cond
        ((< M N)
            (write M)
            (write-char #\Space)
            (rango (+ M 1) N)
        )
        ((eq M N)
            (write N)
        )
    )
)
(rango 1 3)

(defun ordenarl (L)
    
)

(defun longitud (L)
    (setq Cant 0)
    (cond
        ((eq L NIL)
            (return-from Cant)
        )
        (T
            (setq Cant (1+ Cant))
            (longitud (cdr L))
        )
    )
)
(longitud '(a b c d))

;recordar que NIL = [] en este caso
(defun partes (L)
      (cond 
            ((null L) (NIL))
            
            (T
                (setq PX (partes(cdr L)))
                (setq Comb (combinar(car L), PX))
                (append Comb, PX)

            )
         
      )
)
;hay que verificar que X sea un átomo y no otra cosa, ATOM verifica esto
;Otro drama, parece que APPEND no une elementos atómicos y listas, habria que ver esto, se me ocurre crear listas de un elemento para 
;saltar esta limitación, pero hay que probarlo
;dudo si hacer cosas como '((X)) es correcto para decir "La lista que contiene a X como elemento"
(defun combinar(X,L)
        
     (cond
             (atom X)
             ((null L) (list NIL))

             (T
                (setq Rta (append ('((car L)), combinar(X,(cdr L)))))
                (append '((X)),Rta)

            )
     )
 )
