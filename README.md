# Cellular Automata Editor

## Description

This is a small cellular automata editor made for fun and education.

## Usage

CAEditor uses Scheme language to set automaton rules. You should describe `cell-update` function with two arguments: a value of the current cell itself and a list of the values of the [Moore neighborhood](https://en.wikipedia.org/wiki/Moore_neighborhood) of the current cell. The function must return a new value of the current cell.

### Examples

#### John Conway's Game of Life

https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

```scheme
(use-modules (srfi srfi-1))

(define (cell-update itself neighbours)
  (let ((neighbour-sum (fold + 0 neighbours)))
    (cond ((= itself 0) (if (= 3 neighbour-sum) 1 0))
          ((= itself 1) (if (or (= 2 neighbour-sum) (= 3 neighbour-sum)) 1 0)))))
```

https://github.com/d1sxgRee/CAEditor/blob/master/automata/lifegame.scm

#### Wireworld

https://en.wikipedia.org/wiki/Wireworld

```scheme
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
```

https://github.com/d1sxgRee/CAEditor/blob/master/automata/wireworld.scm
