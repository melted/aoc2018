;; day 8
(load "util.ss")

(define-record-type tree (fields children metadata) (nongenerative))

(define (parse-data port)
  (define raw
    (let loop ((acc '()))
      (let ((d (get-datum port)))
        (if (eof-object? d)
          (reverse acc)
          (loop (cons d acc))))))
  (define (parse xs)
    (define nchild (car xs))
    (define nmeta (cadr xs))
    (let loop ((i nchild) (children '()) (l (cddr xs)))
      (if (= i 0)
        (cons (make-tree (reverse children) (list-head l nmeta)) 
              (list-tail l nmeta))
        (let ((tree-tail (parse l)))
          (loop (- i 1) (cons (car tree-tail) children) (cdr tree-tail))))))
  (parse raw))

(define data (car (call-with-input-file "../data/input8.txt" parse-data)))

(define (sum-metadata tree)
  (+ (fold-left + 0 (map sum-metadata (tree-children tree)))
      (fold-left + 0 (tree-metadata tree))))

(define (weird-sum tree)
  (define nchild (length (tree-children tree)))
  (define (index i)
    (if (> i nchild)
      0
      (weird-sum (list-ref (tree-children tree) (- i 1)))))
  (if (= nchild 0)
    (fold-left + 0 (tree-metadata tree))
    (fold-left + 0 (map index (tree-metadata tree)))))

(dump (sum-metadata data) (weird-sum data))