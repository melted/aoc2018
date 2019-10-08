;; day 1 in advent of code 2018

(define vals 
  (call-with-input-file "data/input1.txt"
    (lambda (p)
      (let loop ((vs '()))
        (let ((d (get-datum p)))
          (if (eof-object? d)
            (reverse vs)
            (loop (cons d vs))))))))

(display (fold-left + 0 vals))
(newline)

(define (solve2)
  (define seen (make-eq-hashtable))
  (let loop ((l vals) (acc 0))
    (if (hashtable-contains? seen acc)
      acc
      (let ((next (+ acc (car l)))
            (nl (if (null? (cdr l)) vals (cdr l))))
          (hashtable-set! seen acc #t)
          (loop nl next)))))

(display (solve2))
(newline)