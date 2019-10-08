;; day3
(load "scheme/util.ss")
 
(define vals 
  (call-with-input-file "data/input3.txt"
    (lambda (p)
      (let loop ((vs '()))
        (let ((d (get-line p)))
          (if (eof-object? d)
            (reverse vs)
            (loop (cons d vs))))))))