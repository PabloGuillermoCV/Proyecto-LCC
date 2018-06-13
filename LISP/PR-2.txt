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

(defun partes (L)
    
)
