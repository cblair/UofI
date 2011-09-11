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
	#live graph
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

	individuals <- spherical.steady.state(individuals)
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses

	if(avg.fitnesses[i] < 0.01)
	{
		bored <- 1
	}
	i <- i + 1
}

pdf("results.pdf") #init graph to be written to file
par(mfrow = c(2, 1)) #put the two graph.fitness graphs in one window
spherical.graph.fitnesses(avg.fitnesses, NA)
spherical.graph.fitnesses(spherical.means(individuals),NA)
dev.off() #write out graph


Sys.sleep(5)
