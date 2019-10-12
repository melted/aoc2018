;; day 6 of aoc 2018

(load "util.ss")

(define-record-type point 
  (fields x y)
  (nongenerative))

(define-record-type poi (parent point)
  (fields (mutable score) (mutable finite?))
  (nongenerative))

(define (data->point str)
  (define xy (map string->number (map trim (string-split str ","))))
  (make-poi (car xy) (cadr xy) 0 #t))

(define data (map data->point (get-lines "../data/input6.txt")))

(define (distance p1)
  (lambda (p2)
    (+ (abs (- (point-x p1) (point-x p2)))
      (abs (- (point-y p1) (point-y p2))))))

(define min-x (apply min (map point-x data)))
(define max-x (apply max (map point-x data)))

(define min-y (apply min (map point-y data)))
(define max-y (apply max (map point-y data)))

(define (solve)
  (define region 0)
  (do ((x min-x (+ x 1)))
        ((= x max-x))
    (do ((y min-y (+ y 1)))
        ((= y max-y))
          (let* ((p (make-point x y))
                (dists (map (distance p) data))
                (low (cdr (car (list-sort 
                      (lambda (a b) (< (car a) (car b)))
                      (map cons dists data))))))
            (when (< (fold-left + 0 dists) 10000)
               (set! region (+ region 1)))
            (if (or (= x min-x) (= x max-x) (= y min-y) (= y max-y))
              (poi-finite?-set! low #f)
              (when (poi-finite? low)
                 (poi-score-set! low (+ (poi-score low) 1)))))))
  (cons (apply max (map poi-score (filter poi-finite? data))) region))

(display (solve))
            