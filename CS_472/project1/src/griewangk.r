fitness.func <- function(individual)
{
	A <- 10
	n <- length(individual)

	sum1 <- lapply(1:n, function(i) {
		xi <- individual[i]
		return( (xi ^ 2) / 4000)
	})

	prod1 <- lapply(1:n, function(i) {
		xi <- individual[i]
		return(cos(xi / sqrt(i)))
	})
	
	sum1 <- unlist(sum1)
	sum1 <- sum(sum1)

	prod1 <- unlist(prod1)
	prod1 <- prod(prod1)

	retval <- 1 + sum1 - prod1
	
	return(retval)
}

LB <- -600
UB <- 600
rand.val <- function(lb, ub)
{
	retval <- sample(lb:ub,1)[1]

	return(retval)
}
