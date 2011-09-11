#!/usr/bin/env R

spherical.rand.val <- function()
{
	'#skipping floating point random values...
	lb <- -5.12 #lower bound of random values
	ub <- 5.12 #upper bound of random values
	retval <- runif(1, lb, ub)
	'
	#and doing randoming reals instead
	lb <- -512
	ub <- 512
	retval <- sample(lb:ub,1)[1] * .01

	return(retval)
}

#arg i does nothing but satisy lapply calls
spherical.create.individual <- function(i, n.cols) 
{
		x <- c() #x is each ind.'s attribute vector
		for(i in 1:n.cols)
		{
			x[i] <- spherical.rand.val()
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
	n <- which(fitnesses == min(fitnesses))[1] #1 if there are multi mins
	print(n)
	#copy first min individual
	min1 <- individuals[[n]]

	#copy second max individual
	# [1] if multi second mins
	n <- which(fitnesses == sort(fitnesses,partial=2)[2])[1]
	min2 <- individuals[[n]]

	#make a child
	new.child <- spherical.one.point.crossover(min1,min2)
	
	#mutate the min's
	new.child <- spherical.mutate(new.child)

	#replace first max
	n <- which(fitnesses == max(fitnesses))[1] #1 if there are multi maxes
	individuals[[n]] = new.child
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses

	#replace second max
	#n <- which(fitnesses == max(fitnesses))[1] #1 if there are multi maxes
	#individuals[[n]] = min2
	

	return(individuals)	
}


spherical.one.point.crossover <- function(parent1,parent2)
{
	#determine the point to crossover
	# hopefully both parent lengths are equal!

	#pick a random point in the point
	n <- sample(1:length(parent1),1)

	#first 'half' from parent1
	new.child <- parent1[1:n] 
	#second 'half' from parent2
	new.child <- c(new.child, parent2[n + 1 : length(parent2)])
	#clear out NA's from above
	new.child <- new.child[!is.na(new.child)]
	
	return(new.child)
}


spherical.mutate <- function(individual)
{
	n.cols <- length(individual)

	#pick a random element of the attribute vector, and change its value
	# randomly
	#individual[sample(1: 1)] <- 
	#	runif(1, lb/n.cols, ub/n.cols)
	individual[sample(1:length(individual), 1)] <- spherical.rand.val()

	return(individual)
}
