fitness.func <- function(individual)
{
	A <- 10
	n <- length(individual)

	retval <- lapply(1:n, function(i) {
		xi <- individual[i]
		return( xi ^ 2 - ( A * cos(2 * pi * xi)))
	})
	
	
	retval <- unlist(retval)
	retval <- sum(retval)
	retval <- A * n + retval
	
	return(retval)
}

rand.val <- function()
{
	lb <- -512
	ub <- 512
	retval <- sample(lb:ub,1)[1] * .01

	return(retval)
}
