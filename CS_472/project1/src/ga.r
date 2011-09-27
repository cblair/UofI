#!/usr/bin/env R

FIT.FUNC <- NA
RAND.VAL <- NA
FITNESSES <- NA

GA_K <- 5 #K used during selection


decimalplaces <- function(x) {
    if ((x %% 1) != 0) {
        nchar(strsplit(sub('0+$', '', as.character(x)), ".", fixed=TRUE)[[1]][[2]])
    } else {
        return(0)
    }
}


ga.rand.val <- function(lb,ub)
{
	return(RAND.VAL(lb,ub))
}

#arg i does nothing but satisy lapply calls
ga.create.individual <- function(i, n.cols) 
{
		x <- c() #x is each ind.'s attribute vector
		for(i in 1:n.cols)
		{
			x[i] <- ga.rand.val(LB,UB)
		}
		
		return(x)
}


ga.get.init.population <- function(n.individuals, n.cols)
{
	#make a list[[]] of individuals
	individuals <- lapply(1:n.individuals, ga.create.individual,
				n.cols)

	return(individuals)
}


ga.fitness <- function(individual)
{
	return(FIT.FUNC(individual))
}


ga.fitnesses <- function(individuals)
{
	retval <- lapply(1:length(individuals), function(i) {
		return(ga.fitness(individuals[[i]]))
	})

	return(unlist(retval))
}


ga.means <- function(individuals)
{
	retval <- lapply(1:length(individuals), function(i) {
		return(mean(individuals[[i]]))
	})

	return(unlist(retval))
}


ga.graph.fitnesses <- function(fitnesses,yrange)
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

ga.save.graph <- function(name, fitnesses)
{
	pdf(paste(name,".pdf",sep="")) #init graph to be written to file
	ga.graph.fitnesses(fitnesses, NA)
	dev.off() #write out graph
}


#returns two of the min individuals from a subset of all individuals
ga.selection.one <- function(individuals)
{
	#take a small subset of the individuals - a vector of indices
	# there's sure to be a better way to get this
	subset_individuals <- list()
	for(i in 1:GA_K)
	{
		subset_individuals[[i]] <- individuals[[sample(1:length(individuals),1)]]
	}
	individuals <- subset_individuals

	#Recalculate fitnesses
	fitnesses <- ga.fitnesses(individuals)

	#copy first min individual
	n <- which(fitnesses == min(fitnesses))[1] #1 if there are multi mins
	min1 <- individuals[[n]]

	#copy second max individual
	# [1] if multi second mins
	n <- which(fitnesses == sort(fitnesses,partial=2)[2])[1]
	min2 <- individuals[[n]]

	return(list(min1, min2))
}


ga.steady.state <- function(individuals, elitism)
{
	#Recalculate fitnesses
	FITNESSES <<- ga.fitnesses(individuals)

	
	#Selection
	parents <- ga.selection.one(individuals)

	
	#Re-generate / populate population
	for(i in 1:length(parents))
	{
		#pick random parents
		p1 <- parents[[sample(1:length(parents),1)]]
		p2 <- parents[[sample(1:length(parents),1)]]

		#make a child
		new.child <- ga.one.point.crossover(p1,p2)
		#new.child <- ga.two.point.crossover(min1,min2)

		#Mutate
		# pick a random individual to replace
		n <- sample(1:length(individuals),1)
		individuals[[n]] <- ga.creep.mutate.one(new.child)
	}


	#Elitism!
	if(elitism == TRUE)
	{
		#1 if there are multi mins
		n <- which(FITNESSES == min(FITNESSES))[1]
		#copy first min individual
		min1 <- individuals[[n]]

		#copy second max individual
		# [1] if multi second mins
		n <- which(FITNESSES == sort(FITNESSES,partial=2)[2])[1]
		min2 <- individuals[[n]]

		#replace first max
		# 1 if there are multi maxes
		n <- which(FITNESSES == max(FITNESSES))[1]
		individuals[[n]] = min1
		#recalculate fitnesses
		FITNESSES <- ga.fitnesses(individuals)

		#replace second max
		# 1 if there are multi maxes
		n <- which(FITNESSES == max(FITNESSES))[1] 
		individuals[[n]] = min2
	}

	return(individuals)	
}


ga.generational <- function(individuals, elitism)
{
	#Recalculate fitnesses
	FITNESSES <<- ga.fitnesses(individuals)
	
	#Selection
	parents <- ga.selection.one(individuals)


	#Re-generate / populate population
	new.individuals <- lapply(1:length(individuals), function(n) {
		#pick random parents
		p1 <- parents[[sample(1:length(parents),1)]]
		p2 <- parents[[sample(1:length(parents),1)]]

		#make a child
		new.child <- ga.one.point.crossover(p1,p2)
		#new.child <- ga.two.point.crossover(min1,min2)

		#Mutate
		# mutate the new child
		#return(ga.uniform.mutate(new.child))
		return(ga.creep.mutate.one(new.child))
		#return(ga.creep.mutate.two(new.child))
	})


	#Elitism!
	if(elitism == TRUE)
	{
		#1 if there are multi mins
		n <- which(FITNESSES == min(FITNESSES))[1]
		#copy first min individual
		min1 <- individuals[[n]]

		#copy second max individual
		# [1] if multi second mins
		n <- which(FITNESSES == sort(FITNESSES,partial=2)[2])[1]
		min2 <- individuals[[n]]

		#replace first max
		# 1 if there are multi maxes
		n <- which(FITNESSES == max(FITNESSES))[1]
		new.individuals[[n]] = min1
		#recalculate fitnesses
		FITNESSES <- ga.fitnesses(new.individuals)

		#replace second max
		# 1 if there are multi maxes
		n <- which(FITNESSES == max(FITNESSES))[1] 
		new.individuals[[n]] = min2
	}

	return(new.individuals)	
}


ga.one.point.crossover <- function(parent1,parent2)
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


ga.two.point.crossover <- function(parent1, parent2)
{
	#make sure m != 1 , and n : m is at least 1 : 3
	m <- 1
	while(m < 3 || m == length(parent1))
	{
		m <- sample(1:length(parent1),1)
	}

	#make sure n < m !
	n <- m
	while(n == m || n == 1)
	{
		n <- sample(1:m,1)
	}

	#n now < m
	new.child <- c(parent1[1:n], parent2[(n+1):m], parent1[(m+1):length(parent1)])

	return(new.child)
}


ga.uniform.mutate <- function(individual)
{
	#pick a random element of the attribute vector, and change its value
	# randomly
	#individual[sample(1: 1)] <- 
	#	runif(1, lb/n.cols, ub/n.cols)
	individual[sample(1:length(individual), 1)] <- ga.rand.val(LB,UB)

	return(individual)
}


#mutates one value in an individual 
ga.creep.mutate.one <- function(individual)
{
	#Create bounds
	k <- 10  # the reducing factor for lb/ub values for the new mutation
	n <- sample(1:length(individual), 1) #which random variable to mutate
	#lower bounds and upper bounds of muatation
	lb <- individual[n] - (abs(individual[n] - LB) / k )
	ub <- individual[n] + (abs(individual[n] - UB) / k )
	# find max precision
	p <- 0
	p1 <- decimalplaces(LB)
	p2 <- decimalplaces(UB)
	p <- max(c(p1,p2))
	# ~and round bounds to it
	lb <- round(lb, p)
	ub <- round(ub, p)

	#Mutate the individual
	# also round by max precision
	individual[n] <- round(ga.rand.val(lb,ub), p)
	#print(paste(lb,":",ub,":",individual[n]))

	return(individual)
}

ga.creep.mutate.two <- function(individual)
{
	k <- .10 #reducing factor of mutation

	#find max precision
	p <- 0
	p1 <- decimalplaces(LB)
	p2 <- decimalplaces(UB)
	p <- max(c(p1,p2))

	new_individual <- lapply(1:length(individual), function(i) {
		#pick a random point in the search space
		rand.val <- ga.rand.val(LB,UB)	
		#find the difference between it and this attribute value
		rand.dif <- individual[i] - rand.val
		#scale the difference down, keeping precision
		rand.dif <- round(rand.dif * k, p)
	})
	new_individual <- unlist(new_individual)

	return(new_individual)
}
