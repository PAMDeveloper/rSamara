#Script estimating parameters for Samara model
#Authors : Florian Larue, Gregory Beurier, Lauriane Rouan -- (PAM, AGAP, BIOS, CIRAD)

###Set informations for parameter estimation###
source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')
connectDB('D:/PAMStudio/dev/git/Basedonnee_Malick.accdb')

ParamOfInterest <- c("InternodeLengthMax","LeafLengthMax","CoeffLeafWLRatio","SlaMin","CoeffLeafDeath","TilAbility","CoeffTillerDeath","CoeffInternodeMass","CoeffPanicleMass","PanStructMassMax","CoeffPanSinkPop")
MinValue <- c(90,750,0.08,0.0015,0.0005,0.02,0.05,0.05,0.15,3,5)
MaxValue <- c(120,1100,0.15,0.002,0.0015,0.06,0.15,0.1,0.3,8,10)
obsCoef <- c(85.5,910,0.11,0.00135,0.001,0.04,0.1,0.7,0.22,5,8)
MaxIter <- 50
SolTol <- 0.05
###End informations for parameter estimation###

#Install and load packages
list.of.packages <- c("rsamara", "RODBC", "parallel", "DEoptim")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#Get data for simulation
meteo1 <- loadMeteo('4', '2014/07/07', '2014/12/06')
meteo2 <- loadMeteo('4', '2014/07/07', '2014/12/06')
meteo3 <- loadMeteo('4', '2014/07/07', '2014/12/06')
vObs1 <- loadObs('SINT2014S1G9', '621B','2014/07/07', '2014/12/06')
vObs2 <- loadObs('SINT2014S1G9', '621B','2014/07/07', '2014/12/06')
vObs3 <- loadObs('SINT2014S1G9', '621B','2014/07/07', '2014/12/06')

paramInit <- loadSimDetails('SINT2014S1', '621B', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
odbcCloseAll()

paramInitTrans <- paramInit
NbParam <- length(ParamOfInterest)
Bounds <- matrix(c(MinValue,MaxValue),ncol=2)

#First model run and matrix reduction
resulttmp1 <- rsamara::run2DF(paramInit,meteo1)
resulttmp2 <- rsamara::run2DF(paramInit,meteo2)
resulttmp3 <- rsamara::run2DF(paramInit,meteo3)

obsRed1 <- rsamara::rcpp_reduceVobs(vObs1, resulttmp1)
obsRed2 <- rsamara::rcpp_reduceVobs(vObs2, resulttmp2)
obsRed3 <- rsamara::rcpp_reduceVobs(vObs3, resulttmp3)

resRed1 <- rsamara::rcpp_reduceResults(resulttmp1, vObs1)
resRed2 <- rsamara::rcpp_reduceResults(resulttmp2, vObs2)
resRed3 <- rsamara::rcpp_reduceResults(resulttmp3, vObs3)

#Functions
Optim_Samara_funct <- function(p){
  paramInitTrans[ParamOfInterest] <- p
  parameters <- paramInitTrans

  Res_samara1 <- rsamara::run2DF(parameters,meteo1)
  Res_samara2 <- rsamara::run2DF(parameters,meteo2)
  Res_samara3 <- rsamara::run2DF(parameters,meteo3)

  res1 <- rsamara::rcpp_reduceResults(Res_samara1, vObs1)
  res2 <- rsamara::rcpp_reduceResults(Res_samara2, vObs2)
  res3 <- rsamara::rcpp_reduceResults(Res_samara3, vObs3)


  diff1 <- ((obsRed1[-nrow(obsRed1),] - res1[-nrow(obsRed1),])/obsRed1[-nrow(obsRed1),])^2
  rmae1 = sum(sqrt((colSums(diff1, na.rm=T))/(colSums(!is.na(diff1)))),na.rm=T)

  diff2 <- ((obsRed2[-nrow(obsRed2),] - res2[-nrow(obsRed2),])/obsRed2[-nrow(obsRed2),])^2
  rmae2 = sum(sqrt((colSums(diff2, na.rm=T))/(colSums(!is.na(diff2)))),na.rm=T)

  diff3 <- ((obsRed3[-nrow(obsRed3),] - res3[-nrow(obsRed3),])/obsRed3[-nrow(obsRed3),])^2
  rmae3 = sum(sqrt((colSums(diff3, na.rm=T))/(colSums(!is.na(diff3)))),na.rm=T)

  diffEnd1 <- ((obsRed1[nrow(obsRed1),] - res1[nrow(res1),])/obsRed1[nrow(res1),])^2
  rmse1 = (sum(sqrt((colSums(diffEnd1, na.rm=T))/(colSums(!is.na(diffEnd1)))),na.rm=T))

  diffEnd2 <- ((obsRed2[nrow(obsRed2),] - res2[nrow(res2),])/obsRed2[nrow(res2),])^2
  rmse2 = (sum(sqrt((colSums(diffEnd2, na.rm=T))/(colSums(!is.na(diffEnd2)))),na.rm=T))

  diffEnd3 <- ((obsRed3[nrow(obsRed3),] - res3[nrow(res3),])/obsRed3[nrow(res3),])^2
  rmse3 = (sum(sqrt((colSums(diffEnd3, na.rm=T))/(colSums(!is.na(diffEnd3)))),na.rm=T))

  return (rmae1 + rmae2 + rmae3 + rmse1 + rmse2 + rmse3)
}

optimisation <- function(Optimizer, MaxIter, SolTol, NbParam, Bounds, NbEnv, SDate, EDate) {
  set.seed(224)
  resOptim <- DEoptim(Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2], DEoptim.control(VTR=SolTol,itermax=MaxIter, strategy=2, trace=max(1,MaxIter/20)))
  res$optimizer <- "Diffential Evolution Optimization"
  res$par <- resOptim$optim$bestmem
  res$value <- resOptim$optim$bestval
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
    par(mfrow=c(1,1))
    plot(resOptim, type="b")
    par(mfrow=c(1,1))
    plot(resOptim, plot.type="bestvalit",type="l")
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
