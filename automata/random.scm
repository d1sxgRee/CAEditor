(set! *random-state* (random-state-from-platform))   ; Random seed for PRNG
(define (cell-update itself neighbours)
  (random 4))
