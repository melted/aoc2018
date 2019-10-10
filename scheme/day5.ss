;; day 5

(load "util.ss")
(define polymer (car (get-lines "../data/input5.txt")))

(define (boom str)
  (do ((p 0 (+ p 1)))
   ((= p (- (string-length str) 1)) str)
      (let ((a (string-ref str p))
            (b (string-ref str (+ p 1))))
          (when (and (char-ci=? a b) (not (char=? a b)))
            (string-set! str p #\!)
            (string-set! str (+ p 1) #\!)))))

(define (collapse str)
  (list->string (filter char-alphabetic? (string->list str))))

(define (solve1 str)
  (let loop ((s str) (len (string-length str)))
    (let ((ns (collapse (boom s))))
      (if (= len (string-length ns))
        len
        (loop ns (string-length ns))))))

(define (remove-char str)
  (lambda (c)
    (define s (string-copy str))
    (do ((p 0 (+ p 1)))
        ((= p (string-length s)) (collapse s))
          (when (char-ci=? (string-ref s p) c)
            (string-set! s p #\!)))))

(define (solve2)
  (define candidates (map (remove-char polymer) (string->list "abcdefghijklmnopqrstuvxwyz")))
  (car (list-sort < (map solve1 candidates))))
