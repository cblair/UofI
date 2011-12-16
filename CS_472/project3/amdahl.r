options <- commandArgs()
i <- 0
p <- NA
sgtime <- NA
total.time <- NA
for(option in options) {
        i <- i + 1
        print(options[i])
        if(option == "-p"){
                p <- as.numeric(options[i + 1])
        }
	if(option == "-o") {
		sgtime <- as.numeric(options[i + 1])
	}
	if(option == "-t") {
		total.time <- as.numeric(options[i + 1])
	}
}

test <- function(i) {
	return(i)
}

amdahl <- function(workers,overhead,p) {
	return(abs(1 / ( ((1 - p) + (overhead * workers)) + (p / workers) ) ) )
}

print.amdahl <- function(max.workers,overhead,p) {
	for(i in 1:max.workers) {
		print(paste(i,":",amdahl(i,overhead,p)))
	}
}

nworkers <- 12
if(is.na(sgtime)) {
	require(snow)
	c1 <- makeCluster(nworkers,type="MPI")
	stime <- proc.time()[3]
	parLapply(c1, 1:nworkers, test)
	etime <- proc.time()[3]
	runtime <- etime - stime
	sgtime <- (runtime) #scatter - gather time
	sgtime <- sgtime / nworkers
	stopCluster(c1)
}

print(paste("Scatter / gather time:",sgtime))
print(paste("P time:",p))
print(paste("Total time:",total.time))

#calc proportions
p <- p / total.time
o <- sgtime / total.time

print(paste("p proportion :",p))
print(paste("o proportion :",o))

max.workers <- 1600
#print.amdahl(max.workers,o,p)
l <- lapply(1:max.workers, amdahl, o=o,p=p)
l <- unlist(l)
l
pdf("amdahl_graph.pdf") #init graph to be written to file
plot(1:length(l), l, xlab="processors", 
	ylab="speedup",type="l")
dev.off() #write out graph

max_speedup <- max(l)
max_cpu <- which(l == max(l))

print(paste("Max speedup:",max_speedup,"with",max_cpu,"cpus"))
