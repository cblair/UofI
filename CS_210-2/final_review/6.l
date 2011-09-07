(defun numatoms (l) 
	(if (atom l) 0
		 (setq sum (+ 1 (numatoms (cdr l))))
	)
)

(defun numatoms2 (l)
	(setq sum 0)
	(mapcar #'(lambda (z) 
		(+ sum 1)
		 )
	l)
	sum
)

(numatoms '())
(numatoms 'a)
(numatoms '(a (b) ((c))))
(numatoms2 '(a (b) ((c))))

(exit)
