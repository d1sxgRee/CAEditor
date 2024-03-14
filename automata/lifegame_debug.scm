(use-modules (srfi srfi-1))

(define (cell-update itself neighbours)
  (let ((neighbour-sum (fold + 0 neighbours)))
    (cond ((= itself 0) (if (= 3 neighbour-sum) 1 0))
          ((= itself 1) (if (or (= 2 neighbour-sum) (= 3 neighbour-sum)) 1 0))
          (else (begin
                  (display itself)
                  (newline)
                  0)))))