;; day 4

(load "util.ss")

(define vals (get-lines "../data/input4.txt"))

(define sorted (list-sort string<? vals))

(define (process-val v)
  (define parts (list->vector (string-split v "[]-: ")))
  (define type
    (case (vector-ref parts 7)
      (("Guard") 'guard)
      (("falls") 'sleep)
      (("wakes") 'wake)))
  (if (symbol=? type 'guard)
    (vector type 
      (let ((g (vector-ref parts 8)))
        (string->number (substring g 1 (string-length g)))))
    (vector type (string->number (vector-ref parts 5)))))