#lang racket
(define rules #(0 0 1 1 1 0 0 0 1 1 0 1 1 0 1 0 0 1 0 0 0 1 1 1 0 1 0 1 0 1 0 1))

(define (make-start)
  (define init-state (list 1 2 5 7 10 11 14 15 18 19 23 24 25 26 27
                           29 35 38 41 42 44 45 46 48 50 51 52 53
                           60 68 71 72 73 75 77 79 80 82 84 86 87 88
                           92 93 95 96 97 99))
  (define v (make-vector 1024 0))
  (for ((i init-state)) (vector-set! v (+ i 512) 1))
  v)

(define (evolve v)
  (define nv (make-vector 1024 0))
  (define (window v n)
    (λ (i) (for/sum ((j (in-range n))) (* (vector-ref v (+ i j)) (expt 2 j)))))
  (when (for/or ((i '(2 3 1020 1021))) (= (vector-ref v i) 1))
    (error "Overflow!"))
  (define lookup (window v 5))
  (for ((i (in-range 2 1022)))
    (vector-set! nv i (vector-ref rules (lookup (- i 2)))))
  nv)

(define (score v)
  (define len (vector-length v))
  (for/sum ((i (in-range len)))
    (* (vector-ref v i) (- i (/ len 2)))))

(define (shift v)
  (list->vector (reverse (cdr (reverse (cons 0 (vector->list v)))))))

(define (go n v)
  (define seen (make-hash))
  (define (cycle fp m period)
    (define dist (/ (- n m) period))
    (+ (score fp) (* (for/sum ((i (in-vector fp))) i) dist)))
  (define (worker i v)
    (if (hash-has-key? seen v)
        (cycle v i (- i (hash-ref seen v)))
        (if (= n i)
            (score v)
            (begin
              (hash-set! seen (shift v) i)
              (worker (+ i 1) (evolve v))))))
  (worker 0 v))

(go 20 (make-start))
(go 50000000000 (make-start))