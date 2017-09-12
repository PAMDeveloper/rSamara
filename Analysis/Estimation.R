#Script estimating parameters for Samara model
#Returns best value for each parameter, best fitness value, plots
#Authors : Florian Larue, Grégory Beurier, Lauriane Rouan -- (PAM, AGAP, BIOS, CIRAD)

source('D:/PAMStudio_dev/git/rSamara/R/AccessLoader.R')

#Install and load packages
list.of.packages <- c("parallel","pso", "rgenoud", "ABCoptim", "DEoptim")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#Dates management
lct <- Sys.getlocale("LC_TIME"); Sys.setlocale("LC_TIME", "C")

#Data
#direct from simulation table
<<<<<<< .mine
#initSim('D:/PAMStudio_dev/git/BdD_Sorghum_10geno.accdb', '06SB15-fev13-D1')
=======
#initSim('D:/BdD_Sorghum_10geno.accdb', '06SB15-fev13-D1')
>>>>>>> .theirs

#detailled load
#initSimDetails('D:/BdD_Sorghum_10geno.accdb', 'Sotuba-2011-D1', '06SB15_12fev13', 'SotubaPCDA', '270130', '2011/05/01', '2011/12/31')

#separated load
connectDB('D:/PAMStudio_dev/git/BdD_Sorghum_10geno.accdb')
.GlobalEnv$params <- loadSimDetails('Sotuba-2011-D2', '06SB15_12fev13_', 'SotubaPCDA_SV21', '270130', '2011/05/01', '2011/12/31')
.GlobalEnv$meteo <- loadMeteo('270130', '2011/05/01', '2011/12/31')
.GlobalEnv$obs <- loadObs('Sotuba-2011-D2', '06SB15_12fev13', '2011/05/01', '2011/12/31')
odbcCloseAll()


# ESTIMATE
paramInit = params
vObs = obs
paramInitTrans = paramInit

#First model run and matrix reduction
resulttmp = rsamara::run2DF(paramInit, meteo)
obsRed = rsamara::rcpp_reduceVobs(vObs, resulttmp)
resRed = rsamara::rcpp_reduceResults(resulttmp, vObs)

#Parameters to estimate
ParamOfInterest <- c("ca", "tbase")
NbParam <- length(ParamOfInterest)
MinValue <- c(0, 5)
MaxValue <- c(600, 20)
Bounds <- matrix(c(MinValue,MaxValue),ncol=2)

#Optimisation function
Optim_Samara_funct <- function(p){
  paramInitTrans[ParamOfInterest] <- p
  parameters <- paramInitTrans#data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]))
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res = rsamara::rcpp_reduceResults(Res_samara, vObs)
  diff = ((obsRed - res)/obsRed)^2
  return(sum(sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff)))),na.rm=T))
}

##DE
resOptimDE <- DEoptim(Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2], DEoptim.control(VTR=0,itermax=100, strategy=2))

##plots results
varlist = c("totbiom", "pangrawe", "drymatstempop", "leafdm", "lai")
bestp <- as.vector(resOptimDE$optim$bestmem)
paramInitTrans[ParamOfInterest] <- bestp
parameters <- paramInitTrans
Res_samara <- rsamara::run2DF(parameters,meteo)
res = rsamara::rcpp_reduceResults(Res_samara, vObs)
#obsRed$obsplantdate <- vObs$obsplantdate
par(mfrow=c(2, 2))
obsRed[obsRed=="-999"]<-0
#res$obsplantdate <- sapply(res$obsplantdate, function(x){(x=-999) })
#obsRed$obsplantdate <- sapply(obsRed$obsplantdate, fromjulian)
for (v in varlist) {
  yLimMax = 1.1*max(res[[v]],obsRed[[v]])
  yLimMin = min(res[[v]],obsRed[[v]])
  plot(res$dap, res[[v]], type="l", ylab=v, ylim=c(yLimMin,yLimMax))
  points(obsRed$dap, obsRed[[v]], type="p", col="red")
}

#DE specific plots
par(mfrow=c(1,1))
plot(resOptimDE, type="b")
plot(resOptimDE, plot.type="bestvalit",type="l")

#########EXTRA########
### RGENOUD POP ###
# MODIFIER LE CHEMIN SELON LE RETOUR DE tempdir() DANS READ.TABLE, ne pas oublier genoud.pro à la fin
#tempdir()
#pop <- read.table('C:/Users/U108-J~1/AppData/Local/Temp/RtmpOsHrKz/genoud.pro', comment.char = 'G')
#best <- pop[pop$V1 == 1,, drop = FALSE]
#very.best <- as.matrix(best[nrow(best), 3:ncol(best)])

##Proposition pondération des variables
#rmse = sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff))))
#return(sum(rmse*obsCoef), na.rm=T)
##Proposition ecart-type
#diff = ((abs(obsRed - res) - abs(obsET))/obsRed)^2
#rmse = sqrt((colSums(diff, na.rm=T))/(colSums(!is.na(diff))))
#return(sum(rsme*obsCoef), na.rm=T)

###PSOplots
##plot(resOptim4$stats$it, resOptim3$stats$error, log="y", xlab="It", ylab="Erreur", type="l")
#

#ABCplots
#par(mfrow=c(2, 1))
#for (i in 1:ncol(resOptim1$hist)) {
#  plot(resOptim1$hist[,i], ylab=i, xlab="It", type="b")
#}

##Parallel
#no_cores <- max(1, detectCores() - 2)
#cl <- makeCluster(no_cores, outfile="clusterlog.txt")
#clusterEvalQ(cl, library(recomeristem,rgenoud))
#clusterExport(cl, varlist = c("meteo","vObs","obsRed", "paramInitTrans", "ParamOfInterest", "ParamList"))

##GENOUD
#resOptimGA <- genoud(Optim_Samara_funct, NbParam, max=FALSE, pop.size=1000, max.generations=1000, wait.generations=100, hard.generation.limit=TRUE, MemoryMatrix=TRUE, starting.values=NULL, Domains=Bounds, print.level = 2, boundary.enforcement = 0, gradient.check=FALSE, cluster=cl)

##PSO
#resOptimPSO <- psoptim(rep(NA,NbParam), Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2], control=list(maxit=10000,abstol=0))

##ABC
#resOptimABC <- abc_optim(Bounds[,1], Optim_Samara_funct, lb=Bounds[,1], ub=Bounds[,2], maxCycle = 1000, criter=10000, limit = 500)

##DE
#resOptimDE <- DEoptim(Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2], DEoptim.control(VTR=0,itermax=2000, strategy=2, cluster=cl, packages=c("recomeristem"), parVar=c("meteo","vObs","obsRed", "paramInitTrans", "ParamOfInterest", "ParamList") ))

#stopCluster(cl)

#change parameter value
#parameters[which(parameters=="nbleaf_pi"),2] = 18
#parameters[which(parameters=="nb_leaf_stem_elong"),2] = 9
#parameters[which(parameters=="phenostage_to_end_filling"),2] = 30
#parameters[which(parameters=="phenostage_to_maturity"),2] = 31
