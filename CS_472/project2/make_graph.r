#fname <- 'results.txt'
fname <- 'func.txt'
t <- read.table(fname, header=TRUE, sep=",")
print(t)

library(scatterplot3d)
scatterplot3d(t$x,t$y,t$z, main="Func")
#pdf(paste(fname,'.pdf',sep=""))
