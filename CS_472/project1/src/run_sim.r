#Profiler start
Rprof(tmp <- tempfile())
example(glm)


#Source - ga files
source('./ga.r')


#Handle Options
z <- 0
run.ga <- ""
elitism <- FALSE
algorithm.file <- ""
PARALLEL <- FALSE
GRAPH_FILE <- FALSE
nodes <- 1
options <- commandArgs()
for(option in options) {
	z <- z + 1
    	if(option == "--ss"){
  		run.ga <- "steady.state"   
	}
	else if(option == '--g')
	{
		run.ga <- "generational"
	}
	else if(option == '--gf')
	{
		GRAPH_FILE <- TRUE
	}
	else if(option == "--e")
	{
		elitism <- TRUE
	}
	else if(option == "-a")
	{
		algorithm.file <- options[z + 1]
	}
	else if(option == "-k")
	{
		GA_K <- options[z + 1]
	}

	else if(option == "-p") {
		PARALLEL <- TRUE
		nodes <- options[z + 1]
	}
}


#Source - 
# whatever fitness and rand values we want
source(algorithm.file)


#Parallelism - start
C1 <- NA
if(PARALLEL == TRUE) {
	require(Rmpi)
	require(snow)
	C1 <- makeCluster(nodes, type="MPI")
}


#Register - the fitness and random functions from whatever we source above
FIT.FUNC <- fitness.func
RAND.VAL <- rand.val
# compile internal funcs for performance
if(library(compiler, logical.return=TRUE) == TRUE)
{
	FIT.FUNC <- cmpfun(FIT.FUNC)
	RAND.VAL <- cmpfun(RAND.VAL)
}


#Initialize - fitness variables
individuals <- ga.get.init.population(500,30)
avg.fitnesses <- c()
# calculate first fitnesses
FITNESSES <- ga.fitnesses(individuals)


#Graph - init
quartz("Results",8,6) #init graph window for mac
#X11("Results",8,6) #init graph window for linux
par(mfrow = c(2, 1)) #put the two graph.fitness graphs in one window


#Main - loop
bored <- 0
i <- 1
while(bored == 0)
{
	#timing
	stime <- proc.time()[3]


	#Live graph and data
	ga.graph.fitnesses(FITNESSES,NA)
	avg.fitnesses[i] <- mean(FITNESSES)
	n <- which(FITNESSES == min(FITNESSES))[1] 
	print(paste(i," - Avg fitness:",avg.fitnesses[i],
			"min:",min(FITNESSES),"sd:",sd(FITNESSES)))
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
	if(GRAPH_FILE == TRUE)
	{
		if( (i %% 101) == 0 || i == 110 || i == 120 || i == 130 || i == 140)
		{
			n <- length(avg.fitnesses)
			o <- n - 100
			ga.save.graph(paste("avg_",i,sep=""),avg.fitnesses[o:n])
			ga.save.graph(paste("ind_",i,sep=""),FITNESSES)
		}
	}

	
	#Re-run the GA for the next generation
	if(run.ga == "steady.state")
	{
		individuals <- ga.steady.state(individuals, elitism)
	}
	else if(run.ga == "generational")
	{
		individuals <- ga.generational(individuals, elitism)
	}


	#Evaluate if we have converged, for whatever precision
	min_fitnesses <- min(FITNESSES)
	print(paste("Min fitnesses:",min_fitnesses))
	if(min_fitnesses < .00001  || i > 10000)
	{
		bored <- 1
	}
	i <- i + 1

	#timing
	etime <- proc.time()[3]
	ttime <- etime - stime
	print(paste("Re-gen time:", ttime))
}


if(GRAPH_FILE == TRUE)
{
	ga.save.graph('total_ind',FITNESSES)
	ga.save.graph('avg_ind',avg.fitnesses)
}

Sys.sleep(5)


#Parallelism - stop
if(PARALLEL == TRUE) 
{
	stopCluster(C1)
}


#Profiler - summarize
Rprof()
summaryRprof(tmp)
unlink(tmp)
