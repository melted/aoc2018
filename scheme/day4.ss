;; day 4

(load "util.ss")

(define vals (get-lines "../data/input4.txt"))

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

(define dat (map process-val (list-sort string<? vals)))

(define gds (make-eq-hashtable))

(define (fill-table xs)
  (let loop ((l xs))
    (define gid (vector-ref (car l) 1))
    (if (null? (cdr l))
      #f
      (if (symbol=? 'guard (vector-ref (cadr l) 0))
        (loop (cdr l))
        (let inner ((ll (cdr l)))
          (define slp (vector-ref (car ll) 1))
          (define wk (vector-ref (cadr ll) 1))
          (hashtable-update! gds gid (lambda (v) (cons (vector slp wk) v)) '())
          (if (null? (cddr ll))
            #f
            (if (symbol=? 'guard (vector-ref (caddr ll) 0))
              (loop (cddr ll))
              (inner (cddr ll)))))))))

(fill-table dat)

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
    

(define (max-index v)
  (let loop ((n 0) (max 0) (maxi 0))
    (if (= n (vector-length v))
      maxi
      (let ((u (vector-ref v n)))
        (if (> u max)
          (loop (+ n 1) u n)
          (loop (+ n 1) max maxi))))))

(define (solve1)
  (define (sum-cell ts)
    (cons (car ts) (fold-left + 0 (map sleep-time (cdr ts)))))
  (define vs (map sum-cell (vector->list (hashtable-cells gds))))
  (define sorted (list-sort (lambda (v1 v2) (> (cdr v1) (cdr v2))) vs))
  (define gid (car (car sorted)))
  (define sched (create-schedule (hashtable-ref gds gid 0)))
  (* gid (max-index sched)))

(display (solve1))
