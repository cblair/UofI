#!/usr/bin/env R

#arg i does nothing but satisy lapply calls
spherical.create.individual <- function(i, n.cols) 
{
		x <- c() #x is each ind.'s attribute vector
		lb <- -5.12 #lower bound of random values
		ub <- 5.12 #upper bound of random values
		for(i in 1:n.cols)
		{
			x[i] <- runif(1, lb/n.cols, ub/n.cols)
		}
		
		return(x)
}


spherical.get.init.population <- function(n.individuals, n.cols)
{
	#make a list[[]] of individuals
	individuals <- lapply(1:n.individuals, spherical.create.individual,
				n.cols)

	return(individuals)
}


spherical.fitness <- function(individual)
{
	#fitness function for each individual
	return(abs(0 - sum(individual ^ 2)))
}


spherical.fitnesses <- function(individuals)
{
	retval <- lapply(1:length(individuals), function(i) {
		return(spherical.fitness(individuals[[i]]))
	})

	return(unlist(retval))
}


spherical.means <- function(individuals)
{
	retval <- lapply(1:length(individuals), function(i) {
		return(mean(individuals[[i]]))
	})

	return(unlist(retval))
}


spherical.graph.fitnesses <- function(fitnesses,yrange)
{
	if(is.na(yrange))
	{
		plot(1:length(fitnesses), fitnesses, type="l")
	}
	else
	{
		plot(1:length(fitnesses), fitnesses, type="l",ylim=yrange)
	}
}


spherical.steady.state <- function(individuals)
{
	spherical.selection(individuals)
}


spherical.selection <- function(individuals)
{	
	fitnesses <- spherical.fitnesses(individuals)

	'
	#copy first max individual
	max1 <- individuals[[which(fitnesses == max(fitnesses))]]

	#copy second max individual
	n <- length(fitnesses)
	max2 <- individuals[[which(fitnesses == sort(fitnesses,partial=n-1)[n-1])]]

	#mutate the max-es
	max1 <- spherical.mutate(max1)
	max2 <- spherical.mutate(max2)

	#replace first minimum
	n <- which(fitnesses == min(fitnesses))
	individuals[[n]] = max1
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses

	#replace second minimum
	n <- which(fitnesses == min(fitnesses))
	individuals[[n]] = max2
	'

	#copy first max individual
	min1 <- individuals[[which(fitnesses == min(fitnesses))]]

	#copy second max individual
	n <- length(fitnesses)
	min2 <- individuals[[which(fitnesses == sort(fitnesses,partial=2)[2])]]

	#mutate the min'es
	min1 <- spherical.mutate(min1)
	min2 <- spherical.mutate(min2)

	#replace first max
	n <- which(fitnesses == max(fitnesses))
	individuals[[n]] = min1
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses

	#replace second max
	n <- which(fitnesses == max(fitnesses))
	individuals[[n]] = min2
	

	return(individuals)	
}


spherical.mutate <- function(individual)
{
	lb <- -5.12 #lower bound of random values
	ub <- 5.12 #upper bound of random values
	n.cols <- length(individual)

	#pick a random element of the attribute vector, and change its value
	# randomly
	individual[sample(1: 1)] <- 
		runif(1, lb/n.cols, ub/n.cols)
	return(individual)
}
