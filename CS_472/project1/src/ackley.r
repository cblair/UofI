fitness.func <- function(individual)
{
	A <- 20
	n <- length(individual)

	sum1 <- lapply(1:n, function(i) {
		xi <- individual[i]
		return( xi ^ 2)
	})
	sum1 <- unlist(sum1)
	sum1 <- sum(sum1)

	sum2 <- lapply(1:n, function(i) {
		xi <- individual[i]
		return(cos(2 * pi * xi))
	})
	sum2 <- unlist(sum2)
	sum2 <- sum(sum2)

	retval <- A + exp(1) - ( 20 * exp(-0.2 * sqrt((1/n) * sum1)) ) -
			exp( (1/n) * sum2)
	return(retval)
}

LB <- -30
UB <- 30
rand.val <- function(lb,ub)
{
	retval <- sample(lb:ub,1)[1]

	return(retval)
}
