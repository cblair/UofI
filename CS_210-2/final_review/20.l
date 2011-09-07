(defun nestlist (func x n)
	(if (eq n 0) (funcall func x)
		(funcall func x (nestlist func x (- n 1)))
	)
)

(nestlist 'list 'a 3)
(nestlist 'list 'a 0)

(exit)
