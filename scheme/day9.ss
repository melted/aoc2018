;; day 9

(define-record-type marbles 
  (fields (mutable left)
          (mutable right))
  (nongenerative)
  (protocol (lambda (new) (lambda () (new '(0) '())))))

(define (move-left marbles n)
  (let loop ((i n)) 
    (when (not (zero? i))
      (when (null? (marbles-left marbles))
        (marbles-left-set! marbles (reverse (marbles-right marbles)))
        (marbles-right-set! marbles '()))
      (let ((tmp (marbles-left marbles)))
        (marbles-left-set! marbles (cdr tmp))
        (marbles-right-set! marbles (cons (car tmp) (marbles-right marbles))))
      (loop (- i 1)))))

(define (move-right marbles n)
  (let loop ((i n)) 
    (when (not (zero? i))
      (when (null? (marbles-right marbles))
        (marbles-right-set! marbles (reverse (marbles-left marbles)))
        (marbles-left-set! marbles '()))
      (let ((tmp (marbles-right marbles)))
        (marbles-right-set! marbles (cdr tmp))
        (marbles-left-set! marbles (cons (car tmp) (marbles-left marbles))))
      (loop (- i 1)))))

(define (pop-current marbles)
  (let ((x (car (marbles-left marbles))))
    (marbles-left-set! marbles (cdr (marbles-left marbles)))
    x))

(define (push-current marbles n)
  (marbles-left-set! marbles (cons n (marbles-left marbles))))

(define (place marbles n)
  (if (zero? (remainder n 23))
    (begin
      (move-right marbles 7)
      (+ (pop-current marbles) n))
    (begin
      (move-left marbles 2)
      (push-current marbles n)
      0)))

(define (solve n balls)
  (define elves (make-vector n 0))
  (define circle (make-marbles))
  (let loop ((i 1))
    (define elf (remainder (- i 1) n))
    (if (= i balls)
      (apply max (vector->list elves))
      (begin
        (vector-set! elves elf (+ (vector-ref elves elf) (place circle i)))
        (loop (+ i 1))))))

(display (solve 430 71588))
(newline)
(display (solve 430 7158800))