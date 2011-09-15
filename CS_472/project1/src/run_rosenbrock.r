source('./ga.r')

rosenbrock.fitness <- function(individual)
{
	retval <- lapply(1:length(individual) - 1, function(i) {
		xi <- individual[i]
		xi.one <- individual[i + 1]
		return(100 * ( xi.one - xi ^ 2) + (xi - 1) ^ 2)
	})
	
	retval <- unlist(retval)
	retval <- sum(retval)
	return(retval)
}

rosenbrock.rand.val <- function()
{
	'#skipping floating point random values...
	lb <- -5.12 #lower bound of random values
	ub <- 5.12 #upper bound of random values
	retval <- runif(1, lb, ub)
	'
	#and doing randoming reals instead
	lb <- -2048
	ub <- 2048
	retval <- sample(lb:ub,1)[1] * .001

	return(retval)
}

#register the fitness and random functions
FIT.FUNC <- rosenbrock.fitness
RAND.VAL <- rosenbrock.rand.val

#initialize fitness variables
individuals <- ga.get.init.population(500,30)
avg.fitnesses <- c()

#calculate first fitnesses
fitnesses <- ga.fitnesses(individuals)

for(i in 1:length(fitnesses))
{
	print(paste("Fitness for",i,":",fitnesses[[i]]))
}

quartz("Results",8,6) #init graph window for mac
par(mfrow = c(2, 1)) #put the two graph.fitness graphs in one window

bored <- 0
i <- 1
while(bored == 0)
{
	#Live graph
	ga.graph.fitnesses(fitnesses,NA)
	#ga.graph.fitnesses(ga.means(individuals),NA)
	avg.fitnesses[i] <- mean(fitnesses)
	n <- which(fitnesses == min(fitnesses))[1] 
	print(paste(i," - Avg fitness:",avg.fitnesses[i],
			"min:",min(fitnesses)))
	print(individuals[[n]])
	if(i < 101)
	{
		ga.graph.fitnesses(avg.fitnesses, NA)
	}
	else
	{
		n <- length(avg.fitnesses)
		o <- n - 100
		ga.graph.fitnesses(avg.fitnesses[o:n], NA)
	}

	#Period save graph
	if( (i %% 101) == 0 || i == 110 || i == 120 || i == 130 || i == 140)
	{
		n <- length(avg.fitnesses)
		o <- n - 100
		ga.save.graph(paste("avg_",i,sep=""),avg.fitnesses[o:n])
		ga.save.graph(paste("ind_",i,sep=""),fitnesses)
	}

	
	#Re-run the GA for the next generation
	#individuals <- ga.steady.state(individuals)
	individuals <- ga.generational(individuals)
	fitnesses <- ga.fitnesses(individuals) #recalculate fitnesses

	if(min(fitnesses) == 0.000)
	{
		bored <- 1
	}
	i <- i + 1
}

ga.save.graph('total_ind',fitnesses)
ga.save.graph('avg_ind',avg.fitnesses)

Sys.sleep(5)
