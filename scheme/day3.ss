;; day3
(load "util.ss")
 
(define vals (get-lines "../data/input3.txt"))

(define (process-val v)
  (list->vector
    (map string->number
      (filter 
        (lambda (x) (not (string=? "" x))) 
        (string-split v "#x@:, ")))))

(define fabric (make-vector 1000000 0))

(define (index x y)
  (+ (* 1000 x) y))

(define (traverse p f)
  (let ((x (vector-ref p 1))
        (y (vector-ref p 2))
        (dx (vector-ref p 3))
        (dy (vector-ref p 4)))
    (let loop-x ((cx x))
      (let loop-y ((cy y))
        (if (f cx cy)
          #t
          (if (< cy (+ y (- dy 1)))
            (loop-y (+ 1 cy))
            (if (< cx (+ x (- dx 1)))
              (loop-x (+ 1 cx))
              #f)))))))

(define (handle-patch p)
  (traverse p
    (lambda (cx cy)
        (vector-set! fabric (index cx cy) (+ 1 (vector-ref fabric (index cx cy))))
        #f)))
        
(for-each handle-patch (map process-val vals))

(define count (length (filter (lambda (n) (> n 1)) (vector->list fabric))))

(define (check-patch p)
  (not (traverse p 
    (lambda (cx cy)
      (> (vector-ref fabric (index cx cy)) 1)))))

(define non-overlapping (filter check-patch (map process-val vals)))
