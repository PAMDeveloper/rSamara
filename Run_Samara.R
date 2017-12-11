#Install and load packages
list.of.packages <- c("rsamara","RODBC", "ggplot2", "gdata", "gtools", "Rcpp")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#detailled load
source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')
options(digits=18)


### load each info separated
connectDB('D:/PAMStudio/dev/git/BdD_Sorghum_20151107.accdb')

#Simulation depuis la clé d'un fichier Excel
# .GlobalEnv$params <- loadSim("BAMA2014S1621B","D:/PAMStudio/dev/git/rSamara/simulation.xlsx")

#Simulation depuis la base de données directement
 # .GlobalEnv$params <- loadSimDetails('BAMA2014S1', 'CSM63E', 'SotubaPCDA_SV21', '5', '2014/06/01', '2014/11/21')
 # .GlobalEnv$meteo <- loadMeteo('5', '2014/06/01', '2014/11/21')
 # .GlobalEnv$obs <- loadObs('BAMA2014S1G5', 'CSM63E', '2014/06/01', '2014/11/21')

#Boucle de simulations depuis un fichier Xls
 simulist = read.xls("D:/PAMStudio/dev/git/rSamara/simulation.xlsx")
 launchFromList <- function (simcode, itkcode, variety, fieldcode, wscode, startingdate, endingdate) {
    print(simcode)
    meteoxls <- loadMeteo(wscode,startingdate,endingdate)
    paramsXls = loadSimDetails(itkcode,variety,fieldcode,wscode,startingdate,endingdate)
    xlsresults = rsamara::run2DF(paramsXls,meteoxls)
    filename = paste(simcode, ".csv")

    write.table(format(xlsresults, digits=18), filename, sep='\t',row.names=F)
    print(paste(simcode, " written"))
 }
 mapply(launchFromList, simulist$simcode, simulist$itkcode, simulist$variety, simulist$fieldcode, simulist$wscode, simulist$startingdate, simulist$endingdate)

 ### load from sim Table
 # initSim('D:/PAMStudio_dev/git/BdD_Sorghum_10geno.accdb', '06SB15-fev13-D1_SV21')

 odbcCloseAll()

 results_Samara =rsamara::run2DF(params,meteo)

 ## Write Results
 write.table(format(results_Samara, digits=18), 'Res_R.csv', sep=';',row.names=F)

 ## Plot all results
 allPlot <- function() {
   plotF <- function(x) {
     plot(results_Samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(1.3*min(results_Samara[[x]], na.rm=T),1.3*max(results_Samara[[x]], na.rm=T)))
     return("Done")
   }
   sapply(names(results_Samara), plotF)
 }
 allPlot()


