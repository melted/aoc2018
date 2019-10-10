;; day 4

(load "util.ss")

(define vals  (list-sort string<? (get-lines "../data/input4.txt")))

(define (parse-data vs) 
  (define gds (make-eq-hashtable))
  (define (parse str)
    (define parts (list->vector (string-split str "[]-: ")))
    (case (vector-ref parts 7)
      (("Guard") (vector-ref parts 8))
      (("falls" "wakes") (string->number (vector-ref parts 5)))))
  (define parsed (map parse vs))
  (let loop ((l parsed) (curr-gd #f))
    (if (null? l)
      gds
      (let ((a (car l)))
        (if (string? a)
          (loop (cdr l) (string->number (substring a 1 (string-length a))))
          (let ((update (lambda (xs) 
                          (cons (vector a (cadr l)) xs))))
            (hashtable-update! gds curr-gd update '())
            (loop (cddr l) curr-gd)))))))

(define gds (parse-data vals))
(define (sleep-time v)
  (- (vector-ref v 1) (vector-ref v 0)))

(define (add-sleep ts)
  (lambda (v)
    (let loop ((n (vector-ref v 0)))
      (if (= n (vector-ref v 1))
        v
        (begin
          (vector-set! ts n (+ 1 (vector-ref ts n)))
          (loop (+ n 1)))))))

(define (create-schedule vs)
  (define sched (make-vector 60 0))
  (map (add-sleep sched) vs)
  sched)
    

(define (max-index-values v)
  (let loop ((n 0) (max 0) (maxi 0))
    (if (= n (vector-length v))
      (cons maxi max)
      (let ((u (vector-ref v n)))
        (if (> u max)
          (loop (+ n 1) u n)
          (loop (+ n 1) max maxi))))))

(define (max-index v)
  (car (max-index-values v)))

(define (solve1)
  (define (sum-cell ts)
    (cons (car ts) (fold-left + 0 (map sleep-time (cdr ts)))))
  (define vs (map sum-cell (vector->list (hashtable-cells gds))))
  (define sorted (list-sort (lambda (v1 v2) (> (cdr v1) (cdr v2))) vs))
  (define gid (car (car sorted)))
  (define sched (create-schedule (hashtable-ref gds gid "")))
  (* gid (max-index sched)))

(display (solve1))

(define (solve2)
  (define (prc ys) (cons (car ys) (max-index-values (create-schedule (cdr ys)))))
  (define xs (map prc (vector->list (hashtable-cells gds))))
  (define top (car (list-sort (lambda (a b) (> (cddr a) (cddr b))) xs)))
  (* (car top) (cadr top)))
(newline)
(display (solve2))