m <- matrix(c(.8,.1,.1,.5,.1,.4,.8,.1,.1),nrow=3)

m1 <- m %*% m #matrix multiply m times itself
print(m1)

m2 <- m1 %*% m1 #matrix multiply m times itself
print(m2)

