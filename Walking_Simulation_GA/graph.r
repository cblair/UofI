fitnesses <- read.table("graph_in.dat")

l <- fitnesses$min_fit

pdf("graph.pdf") #init graph to be written to file
plot(1:length(l), l, type="l")
dev.off() #write out graph
