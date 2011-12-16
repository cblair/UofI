fitnesses <- read.table("graph_in.dat")

l <- fitnesses$min_fit

pdf("graph.pdf") #init graph to be written to file
plot(1:length(l), l, xlab="generations", ylab="cost/fitness",type="l")
dev.off() #write out graph

min_fit <- min(l)
max_fit <- max(l)
sd_fit <- sd(l)

paste("Min:",min_fit,", Max:",max_fit,", standard deviation:",sd_fit)
