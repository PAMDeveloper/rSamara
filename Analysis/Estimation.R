#Script estimating parameters for Samara model
#Authors : Florian Larue, Gregory Beurier, Lauriane Rouan -- (PAM, AGAP, BIOS, CIRAD)

###Set informations for parameter estimation###
source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')
connectDB('D:/PAMStudio/dev/git/BdD_Sorghum_20151107.accdb')

ParamOfInterest <- c("co2slopetr", "coeffrescapacityinternode", "krespinternode")
MinValue <- c(-1, 0, 0)
MaxValue <- c(1, 5, 0.5)
obsCoef <- c(1,1,1)
Optimizer <- "D" #(D = DE, G = RGenoud, A = Simulated Annealing)
RmseM <- "RC" #(RS = RSME-sum, REC = RMSE-EC, RC = RMSE-coef, RECC = RMSE-EC-coef)
MaxIter <- 50
SolTol <- 0.05
###End informations for parameter estimation###

#Install and load packages
list.of.packages <- c("rsamara", "RODBC", "parallel","pso", "rgenoud", "ABCoptim", "DEoptim", "truncnorm")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#Dates management
lct <- Sys.getlocale("LC_TIME"); Sys.setlocale("LC_TIME", "C")

# PPath <- "D:/Workspace/R/testestim"
# MPath <- "D:/Workspace/R/testestim"
# VPath <- "D:/Workspace/R/testestim"
# VName <- "vobs.txt"
# VECName <- "vobsEC.txt"
#Get data for simulation
meteo <- loadMeteo('5', '2014/06/01', '2014/11/21')
paramInit <- loadSimDetails('BAMA2014S1', 'CSM63E', 'SotubaPCDA_SV21', '5', '2014/06/01', '2014/11/21')
vObs <- loadObs('BAMA2014S1G5', 'CSM63E', '2014/06/01', '2014/11/21')
odbcCloseAll()

# obsET <- recomeristem::rcpp_get_vObs(paste(VPath,paste("/",VECName, sep=""), sep=""))
# ParamList <- as.character(names(paramInit))
# paramInitTrans <- as.data.frame(matrix(ncol=length(ParamList)))
# colnames(paramInitTrans) <- ParamList
# paramInitTrans[1,] <- paramInit$Values
paramInitTrans <- paramInit
NbParam <- length(ParamOfInterest)
Bounds <- matrix(c(MinValue,MaxValue),ncol=2)

#First model run and matrix reduction
resulttmp <- rsamara::run2DF(paramInit,meteo)
obsRed <- rsamara::rcpp_reduceVobs(vObs, resulttmp)
# obsETRed <- rsamara::rcpp_reduceVobs(obsET, resulttmp)
resRed <- rsamara::rcpp_reduceResults(resulttmp, vObs)
VarList <- names(obsRed)
res <- list()

#Functions
Optim_Samara_funct <- function(p){
  paramInitTrans[ParamOfInterest] <- p
  parameters <- paramInitTrans #data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]))
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res <- rsamara::rcpp_reduceResults(Res_samara, vObs)
  switch(RmseM,
         "RS" = {
           diff <- ((obsRed - res)/obsRed)^2
           return(sum(sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff)))),na.rm=T))
         }
         ,
         # "REC" = {
         #   diff = ((abs(obsRed - res) - abs(obsETRed))/obsRed)^2
         #   return(sum(sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff)))), na.rm=T))
         # }
         # ,
         "RC" = {
           diff <- ((obsRed - res)/obsRed)^2
           rmse = sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff))))
           return(sum(rmse*obsCoef, na.rm=T))
         }
         # ,
         # "RECC" = {
         #   diff = ((abs(obsRed - res) - abs(obsETRed))/obsRed)^2
         #   rmse = sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff))))
         #   return(sum(rmse*obsCoef, na.rm=T))
         # }
         )
}
simulatedAnnealing <- function(func, start_par, lower, upper, itermax = 1000, step = 0.1, printlvl = 100) {
  best_par <- current_par <- neighbor_par <- start_par
  best_value <- current_value <- neighbor_value <- func(start_par)

  message("It\tBest\tCurrent")
  message(sprintf("%i\t%.4f\t%.4f", 0L, best_value, current_value))

  for (k in 1:itermax) {
    temp <- (1 - step)^k
    neighbor_par <- rtruncnorm(length(start_par),lower, upper, current_par, 1)
    neighbor_value <- func(neighbor_par)

    if (neighbor_value < current_value || runif(1,0,1) < exp(-(neighbor_value - current_value) / temp)) {
      current_par <- neighbor_par
      current_value <- neighbor_value
    }

    if (neighbor_value < best_value) {
      best_par <- neighbor_par
      best_value <- neighbor_value
    }

    if(k%%printlvl == 0) {
      message(sprintf("%i\t%.4f\t%.4f", k, best_value, current_value))
    }
  }
  return(list(iterations = itermax, value = best_value, par = best_par))
}
optimisation <- function(Optimizer, MaxIter, SolTol, NbParam, Bounds, NbEnv, SDate, EDate) {
  set.seed(224)
  switch(Optimizer,
         "D" = {
           resOptim <- DEoptim(Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2], DEoptim.control(VTR=SolTol,itermax=MaxIter, strategy=2, trace=max(1,MaxIter/20)))
           res$optimizer <- "Diffential Evolution Optimization"
           res$par <- resOptim$optim$bestmem
           res$value <- resOptim$optim$bestval
         },
         "G" = {
           resOptim <- genoud(Optim_Samara_funct, NbParam, max=FALSE, pop.size=100, max.generations=MaxIter, wait.generations=max(10,MaxIter/10), hard.generation.limit=TRUE, MemoryMatrix=TRUE, starting.values=NULL, Domains=Bounds, print.level = 2, boundary.enforcement = 0, gradient.check=FALSE, solution.tolerance=SolTol)
           res$optimizer <- "Genetic Optimization Using Derivatives"
           res$par <- resOptim$par
           res$value <- resOptim$value
         },
         "A" = {
           resOptim <- simulatedAnnealing(Optim_Samara_funct, start_par=Bounds[,1], lower=Bounds[,1], upper=Bounds[,2], itermax=MaxIter, step=SolTol, printlvl=max(1,MaxIter/20))
           res$optimizer <- "Simulated Annealing"
           res$par <- resOptim$par
           res$value <- resOptim$value
         }
  )
  print("End of estimation, type res for optimisation results, type resPlot() to see plots of observation variables, allPlot() to see plots of all computed variables and dePlot() for convergence plot (in case of optimizer = D)")
  return(list(res,resOptim))

}
resPlot <- function() {
  bestp <- as.vector(res$par)
  paramInitTrans[ParamOfInterest] <- bestp
  parameters <- paramInitTrans #data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]), row.names=NULL)
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res = rsamara::rcpp_reduceResults(Res_samara, vObs)
  obsRed$day <- vObs$day

  plotF <- function(x) {
    plot(Res_samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(min(min(Res_samara[[x]], na.rm=T),min(obsRed[[x]], na.rm=T)),max(max(Res_samara[[x]], na.rm=T),max(obsRed[[x]], na.rm=T))))
    points(obsRed$day, obsRed[[x]], type="p", col="red")
    # if(!is.null(obsETRed[[x]])) {
    #   arrows(obsRed$day,obsRed[[x]]-obsETRed[[x]],obsRed$day,obsRed[[x]]+obsETRed[[x]], code=3, length=0.02, angle = 90)
    # }
    if(!is.null(obsRed[[x]])) {
      return(paste((abs(sum((abs((obsRed[[x]] - res[[x]])))/(obsRed[[x]]), na.rm=T))*100),"%", sep=""))
    } else {
      return(NULL)
    }
  }
  sapply(VarList, plotF)
}
dePlot <- function() {
  if(Optimizer == "D") {
    par(mfrow=c(1,1))
    plot(resOptim, type="b")
    par(mfrow=c(1,1))
    plot(resOptim, plot.type="bestvalit",type="l")
  }
}
allPlot <- function() {
  bestp <- as.vector(res$par)
  paramInitTrans[ParamOfInterest] <- bestp
  parameters <- paramInitTrans#data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]), row.names=NULL)
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res = rsamara::rcpp_reduceResults(Res_samara, vObs)
  obsRed$day <- vObs$day

  plotF <- function(x) {
    plot(Res_samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(1.5*min(Res_samara[[x]], na.rm=T),1.5*max(Res_samara[[x]], na.rm=T)))
    return("Done")
  }
  sapply(names(Res_samara), plotF)
}

time <- system.time(resOptim <- optimisation(Optimizer, MaxIter, SolTol, NbParam, Bounds, NbEnv, SDate, EDate))
res <- resOptim[[1]]
res$time <- time
resOptim <- resOptim[[2]]
