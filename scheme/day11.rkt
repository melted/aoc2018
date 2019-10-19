#lang racket
;; day 11

(define (power-level x y)
  (let* ((rack (+ x 10))
         (val (* rack (+ (* rack y) 7315))))
    (- (remainder (quotient val 100) 10) 5)))

(define (sum-levels x y n)
  (for*/fold ((acc 0))
             ((dx (in-range x (+ x n)))
              (dy (in-range y (+ y n))))
    (+ acc (power-level dx dy))))
 
(define (power-levels n)
  (for*/list ((x (in-range 1 (- 301 n))) (y (in-range 1 (- 301 n))))
                 (list (sum-levels x y n) x y n)))

(define (solve n)
  (car (sort (power-levels n) (λ (a b) (> (car a) (car b))))))

(solve 3)

(car (sort (map solve (range 1 30)) (λ (a b) (> (car a) (car b)))))