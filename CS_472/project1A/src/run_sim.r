source('./spherical_ga.r')

individuals <- spherical.get.init.population(500,30)

avg.fitnesses <- c()

fitnesses <- spherical.fitnesses(individuals)
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
	spherical.graph.fitnesses(fitnesses,NA)
	#spherical.graph.fitnesses(spherical.means(individuals),NA)
	avg.fitnesses[i] <- mean(fitnesses)
	n <- which(fitnesses == min(fitnesses))[1] 
	print(paste(i," - Avg fitness:",avg.fitnesses[i],
			"min:",min(fitnesses)))
	print(individuals[[n]])
	if(i < 101)
	{
		spherical.graph.fitnesses(avg.fitnesses, NA)
	}
	else
	{
		n <- length(avg.fitnesses)
		o <- n - 100
		spherical.graph.fitnesses(avg.fitnesses[o:n], NA)
	}

	#Period save graph
	if( (i %% 101) == 0 || i == 110 || i == 120 || i == 130 || i == 140)
	{
		n <- length(avg.fitnesses)
		o <- n - 100
		spherical.save.graph(paste("avg_",i,sep=""),avg.fitnesses[o:n])
		spherical.save.graph(paste("ind_",i,sep=""),fitnesses)
	}

	
	#Re-run the GA for the next generation
	#individuals <- spherical.steady.state(individuals)
	individuals <- spherical.generational(individuals)
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses

	if(min(fitnesses) <= 0.0)
	{
		bored <- 1
	}
	i <- i + 1
}

spherical.save.graph('total_ind',fitnesses)
spherical.save.graph('avg_ind',avg.fitnesses)
#pdf("results.pdf") #init graph to be written to file
#par(mfrow = c(2, 1)) #put the two graph.fitness graphs in one window
#spherical.graph.fitnesses(avg.fitnesses, NA)
#spherical.graph.fitnesses(spherical.means(individuals),NA)
#dev.off() #write out graph


Sys.sleep(5)
