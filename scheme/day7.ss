;; day 7 

(load "util.ss")

(define (parse-data str) 
  (define xs (string-split str " "))
  (cons (list-ref xs 1) (list-ref xs 7)))

(define data (map parse-data (get-lines "../data/input7.txt")))

(define (make-blocklist)
  (define blocks (make-hashtable string-hash string=?))
  (define (add-block p)
    (hashtable-update! 
      blocks 
      (cdr p)
      (lambda (x)
        (cons (car p) x))
      '()))
  (map add-block data)
  blocks)

(define (find-available xs)
  (define available (make-hashtable string-hash string=?))
  (define (add e)
    (hashtable-set! available (car e) #t)
    (hashtable-set! available (cdr e) #t))
  (for-each add xs)
  (list-sort string<? (vector->list (hashtable-keys available))))

(define (solve)
  (define start (find-available data))
  (define blocks (make-blocklist))
  (define (remove-block c)
    (vector-for-each (lambda (k) 
          (hashtable-update! blocks k (lambda(xs) (remove c xs)) '())) (hashtable-keys blocks)))
  (define (worker left acc)
    (if (null? left)
      (reverse acc)
      (let ((unblocked (find (lambda (n) (null? (hashtable-ref blocks n '()))) left)))
        (if unblocked
          (begin
            (remove-block unblocked)
            (worker (remove unblocked left) (cons unblocked acc)))
          #f))))
  (fold-left string-append "" (worker start '())))



