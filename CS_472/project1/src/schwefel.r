fitness.func <- function(individual)
{
	n <- length(individual)

	retval <- lapply(1:n, function(i) {
		xi <- individual[i]
		return( xi * (sin(sqrt(abs(xi)))))
	})
	
	
	retval <- unlist(retval)
	retval <- sum(retval)
	retval <- 418.9829 * n + retval
	
	return(retval)
}


LB <- -512.03
UB <- 511.97
rand.val <- function(lb, ub)
{
	'#skipping floating point random values...
	lb <- -5.12 #lower bound of random values
	ub <- 5.12 #upper bound of random values
	retval <- runif(1, lb, ub)
	'
	#and doing randoming reals instead
	lb <- lb * 100
	ub <- ub * 100
	retval <- sample(lb:ub,1)[1] * .01

	return(retval)
}
