#source('./rastrigin.r')
#source('./ackley.r')
#source('./griewangk.r')
i <- rep(0,30)

source('./schwefel.r')
i <- rep(-420.9687,30)

#source('./rosenbrock.r')
#i <- rep(1,30)

PARALLEL <- FALSE


fitness.func(i)
