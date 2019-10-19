#lang racket
;; day 10

(struct pos (x y) #:transparent)

(struct spark pos (dx dy) #:transparent)

(define (parse-data str)
  (apply spark (map string->number
       (cdr (regexp-match #rx"position=< *(-?[0-9]+), *(-?[0-9]+)> velocity=< *(-?[0-9]+), *(-?[0-9]+)>" str)))))

(define data (map parse-data (file->lines "../data/input10.txt")))

(define ((spark-at t) sp)
  (pos (+ (pos-x sp) (* t (spark-dx sp)))
       (+ (pos-y sp) (* t (spark-dy sp)))))

(define (spread t)
  (define pos-at-t (map (spark-at t) data))
  (define xs (map pos-x pos-at-t))
  (- (apply max xs) (apply min xs)))

(define (find-time)
  (let loop ((t 0) (last (spread 0)))
    (let ((next (spread (+ t 1))))
      (if (> next last) t (loop (+ t 1) next)))))

(define (draw-points t)
  (define sparks (map (spark-at t) data))
  (define min-x (apply min (map pos-x sparks)))
  (define max-x (+ (apply max (map pos-x sparks)) 1)) 
  (define min-y (apply min (map pos-y sparks)))
  (define max-y (+ (apply max (map pos-y sparks)) 1))
  (define lcd (build-vector (- max-y min-y) (λ(x)(make-string (- max-x min-x) #\.))))
  (define (paint pt)
    (define row (vector-ref lcd (- (pos-y pt) min-y)))
    (string-set! row (- (pos-x  pt) min-x) #\x))
  (for-each paint sparks)
  (vector-map displayln lcd)
  lcd)
 
(define t (find-time))
(displayln t)
(draw-points t)
