(define 
(insert-sort lst)
(define (insert x sorted)
  (if (null? sorted)
   (list x)
        (if (< x (car sorted))(cons x sorted)(cons (car sorted) (insert x (cdr sorted))))))
(if (null? lst)'()(insert (car lst) (insert-sort (cdr lst)))))
(define input-list '(12 71 2 15 29 82 87 8 18 66 81 25 63 97 40 3 93 58 53 31 47))
(print (insert-sort input-list))