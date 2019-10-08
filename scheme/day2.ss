;; day2 of aoc 2018 in scheme

(define vals 
  (call-with-input-file "data/input2.txt"
    (lambda (p)
      (let loop ((vs '()))
        (let ((d (get-line p)))
          (if (eof-object? d)
            (reverse vs)
            (loop (cons d vs))))))))

(define (histogram xs)
  (define counts (make-eq-hashtable))
  (define (update x)
    (hashtable-update! counts x (lambda (n) (+ n 1)) 0))
  (for-each update xs)
  counts)

(define (has n str)
  (let-values (((k v) (hashtable-entries (histogram (string->list str)))))
    (exists (lambda (m) (= m n)) (vector->list v))))

(define (checksum ys)
  (define (count n)
    (length (filter (lambda (s) (has n s)) ys)))
  (* (count 2) (count 3)))

(display (checksum vals))
(newline)

(define (common str1 str2)
  (define (same c1 c2)
    (if (char=? c1 c2) c1 #f))
  (let ((xs (string->list str1))
        (ys (string->list str2)))
    (list->string 
      (filter char? (map same xs ys)))))

(define (small-diff str1 str2)
  (let ((res (common str1 str2)))
    (if (= (- (string-length str1) (string-length res)) 1) res #f)))

(define (solve2)
  (let loop ((str (car vals)) (xs (cdr vals)))
    (let inner ((l xs))
      (let ((s (small-diff str (car l))))
        (if s
          s
          (if (null? (cdr l))
            (if (null? (cdr xs)) #f (loop (car xs) (cdr xs)))
            (inner (cdr l))))))))
  