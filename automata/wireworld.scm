(use-modules (srfi srfi-1))

(define (cell-update itself neighbours)
  (let ((heads (fold
                (lambda
                  (x count)
                  (if (eq? 3 x)
                      (+ count 1)
                      count))
                0 
                neighbours)))
    (cond ((= itself 0) 0)
          ((= itself 1) (if (or (eq? heads 1) (eq? heads 2)) 3 1))
          ((= itself 2) 1)
          ((= itself 3) 2))))