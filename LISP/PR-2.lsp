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


(defun partes (L)
    
)
