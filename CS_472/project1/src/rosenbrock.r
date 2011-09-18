fitness.func <- function(individual)
{
	fit <- function(i) {
		xi <- individual[i]
		xi.one <- individual[i + 1]
		return(100 * ( xi.one - xi ^ 2) + (xi - 1) ^ 2)
	}

	retval <- NA	
	if(PARALLEL)
	{
		retval <- parLapply(C1, 1:length(individual) - 1, fit)
	}
	else
	{
		retval <- lapply(1:length(individual) - 1, fit)
	}

	retval <- unlist(retval)
	retval <- abs(sum(retval))
	
	
	return(retval)
}

LB <- -2.048
UB <- 2.048	
rand.val <- function(lb,ub)
{
	#random reals, scaled back into reduced precision 
	lb <- lb * 1000
	ub <- ub * 1000
	retval <- sample(lb:ub,1)[1] * .001

	return(retval)
}
