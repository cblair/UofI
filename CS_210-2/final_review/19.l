(defun sqr (x)
	(mapcar #'(lambda (z)
			(* z z)  
		  )
	x)
)

(sqr '(4 2 3 5))

(exit)
