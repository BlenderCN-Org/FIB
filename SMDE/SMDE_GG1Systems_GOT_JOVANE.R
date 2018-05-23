#-------------------exercise 1:

set.seed(42)

#parameters
#number of clients
clients = 10000
#shape
a = 0.6500
#a = 0,5556
#scale
b = 1

#THEORY
#firstly we define the expected service time, based on theory
#Mean E[x]
Theor_mean<- b * gamma((a + 1)/a)
#Variance Var[X]
Theor_var <- b^2 *(gamma((a+2)/a) - (gamma((a+1)/a))^2)
#COefficent of variation
Theor_cv <- sqrt(Theor_var/(Theor_mean)^2)


#EXPERIMENT
#then we define our weibull distribution
service_time <-rweibull(clients, a, b)
#in order to analyze the parameters we convert it into a vector
st <- as.vector(service_time)

Obtained_mean <- mean(st)
Obtained_var <- var(st)
Obtained_median <- median(st)
Obtained_cv <- sqrt(var(st))/mean(st)

sprintf("Mean from theory = %s", Theor_mean)
sprintf("Mean from experiment = %s", Obtained_mean)

sprintf("Variance from theory = %s", Theor_var)
sprintf("Vatiance from experiment = %s", Obtained_var)

sprintf("CV from theory = %s", Theor_cv)
sprintf("CV from experiment = %s", Obtained_cv)


sprintf("Median from experiment = %s", Obtained_median)

hist(st)



#-------------------exercise 2:

clients2=100000
traffic_factors=c(0.4,0.7,0.85,0.925)
#shape
a = 0.6500
#a = 0,5556
seeds=c(309,8734,3672,7346,1256,3476,4387,9822,276,384)


for(i in 1:length(traffic_factors)){

  #Initialization
  w <- c()
  wq <- c()
  W <- c()
  Wq <- c()
  L <- c()
  Lq <- c()
  #Lt <- c()
  Lt_seeds <- matrix(nrow = length(seeds), ncol=clients2)
  t_seeds <- matrix(nrow=length(seeds), ncol=clients2)

  AllenCuneen_Wq <- c()
  AllenCuneen_Lq <- c()

  for(seed in 1:length(seeds)) {

    set.seed(seeds[seed])

    #tau <- rgamma(clients2,7,3)
    tau <- rgamma(clients2,32,2)
    Etau <- mean(tau)
    #tau <- c(0,tau)

    #calculation of b with the parameters
    b <- traffic_factors[i] * Etau / gamma((a + 1)/a)

    #x follows a weibull distribution
    x <- as.vector(rweibull(clients2,a,b))

    #t is the cumulative sum of tau
    t <- cumsum(tau)

    #Calculation of each arrival and exit time
    theta <- numeric(length(x))
    ts <- numeric(length(x))

    for (j in 1:clients2){

      #ts[1]=t[1]
      #at j>1, ts[j] = max(theta[j-1], t[j])

      ifelse(j==1, ts[j] <- t[j], ts[j] <- max(theta[j-1],t[j]))

      theta[j] <- ts[j] + x[j]

    }


    #Calculating statistics ------------

    #Sojourn time of clients in WS
    w = theta - t
    #Sojourn time of clients in queue
    wq <- ts - t

    L <- cumsum(w)
    W <- L
    Lq <- cumsum(wq)
    Wq <- Lq

    Lt_seeds[seed,] <- L / (t - t[1])
    t_seeds[seed,] <- t

    W <- W/clients2
    Wq <- Wq/clients2
    L <- L/(t[clients2]-t[1])
    Lq <- Lq/(t[clients2]-t[1])


    #Theoretical values ------------
    #Allen-Cuneen appro

    lambda <- 1/mean(tau)
    mu <- 1/mean(x)
    rho <- traffic_factors[i]

    AllenCuneen_Lq <- (rho^2 / (1-rho)) * (lambda^2 * var(tau) + mu^2 * var(x)) / 4
    AllenCuneen_Wq <- AllenCuneen_Lq / lambda


  }


  #Taking the means of the previous values for all the seeds
  Lt <- apply(Lt_seeds,2,mean)
  t <- apply(t_seeds,2,mean)

  #Standard deviation
  sd_Wq <- sd(Wq)
  sd_Lq <- sd(Lq)

  W <- mean(W)
  Wq <- mean(Wq)
  L <- mean(L)
  Lq <- mean(Lq)
  AllenCuneen_Lq <- mean(AllenCuneen_Lq)
  AllenCuneen_Wq <- mean(AllenCuneen_Wq)


  #Confidence interval
  #mean +/- 2*sd / sqrt(n)
  CI_Wq_low <- Wq - 2*sd_Wq/sqrt(length(seeds))
  CI_Wq_up <- Wq + 2*sd_Wq/sqrt(length(seeds))

  CI_Lq_low <- Lq - 2*sd_Lq/sqrt(length(seeds))
  CI_Lq_up <- Lq + 2*sd_Lq/sqrt(length(seeds))

  #Printing results
  cat(sprintf("rho = %f, W = %f, L = %f, Wq = %f, CI = [%f,%f], Lq = %f,  CI = [%f,%f], Allen Cuneen Lq = %f, Allen Cuneen Wq = %f, b = %f \n",
              traffic_factors[i], W, L, Wq, CI_Wq_low, CI_Wq_up, Lq, CI_Lq_low, CI_Lq_up, AllenCuneen_Lq, AllenCuneen_Wq, b))



  #Plot curves
  plot(t, Lt, type = "n", main = traffic_factors[i])
  lines(t,Lt)


  hist(w,main=traffic_factors[i])
  hist(wq,main=traffic_factors[i])

  summary(w)
  summary(wq)



}
