fitness.func <- function(individual)
{
	return(sum(individual ^ 2))
}


LB <- -5.12
UB <- 5.12
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
