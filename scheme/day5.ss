;; day 5

(load "util.ss")
(define polymer (car (get-lines "../data/input5.txt")))

(define poly2 (string->list polymer))

(define (reduce xs x)
  (if (null? xs)
    (cons x '())
    (let ((top (car xs)))
      (if (and (char-ci=? top x) (not (char=? top x)))
        (cdr xs)
        (cons x xs)))))

(define (alt-solve pp)
  (length (fold-left reduce (list (car pp)) (cdr pp))))

(define (filtered pp)
  (lambda (ch)
    (filter (lambda (c) (not (char-ci=? c ch))) pp)))

(define candidates (map (filtered poly2) (string->list "abcdefghijklmnopqrstuvxwyz")))

(display (alt-solve poly2))
(newline)
(display (car (list-sort < (map alt-solve candidates))))