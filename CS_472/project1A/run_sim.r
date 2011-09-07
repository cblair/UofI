source('./spherical_ga.r')

individuals <- spherical.get.init.population(500,5)

avg.fitnesses <- c()

fitnesses <- spherical.fitnesses(individuals)
for(i in 1:length(fitnesses))
{
	print(paste("Fitness for",i,":",fitnesses[[i]]))
}

quartz() #init graph window for mac
par(mfrow = c(2, 1))

#graph fitnesses
i <- 1
while(1)
{
	#spherical.graph.fitnesses(fitnesses,-1.0:0)
	spherical.graph.fitnesses(spherical.means(individuals),NA)
	avg.fitnesses[i] <- mean(fitnesses)
	print(paste("Avg fitness:",mean(avg.fitnesses[i])))
	spherical.graph.fitnesses(avg.fitnesses, NA)
	individuals <- spherical.steady.state(individuals)
	fitnesses <- spherical.fitnesses(individuals) #recalculate fitnesses
	i <- i + 1
}

Sys.sleep(5)
