########################
######  PACKAGES  ######
########################

list.of.packages <- c("rsamara", "RODBC", "parallel", "stringr")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')


############################
#######   LOAD DATA  #######
############################
DB_PATH <- "D:/Bd_Samara.accdb"
connectDB(DB_PATH)

ncores <- 30

#Boucle de simulations depuis un fichier Xls
simulist = read.xls("D:/PAMStudio/dev/git/rSamara/simulation.xlsx")
launchFromList <- function (simcode, itkcode, variety, fieldcode, wscode, startingdate, endingdate) {
  print(simcode)
  meteo <- loadMeteo(wscode,startingdate,endingdate)
  params = loadSimDetails(itkcode,variety,fieldcode,wscode,startingdate,endingdate)
  results = rsamara::run2DF(params,meteo)

  filename = paste(simcode, ".csv")
  write.table(format(xlsresults, digits=18), filename, sep='\t',row.names=F)
  print(paste(simcode, " written"))
}
result <- mclapply(launchFromList,
       simulist$simcode, simulist$itkcode, simulist$variety, simulist$fieldcode, simulist$wscode, simulist$startingdate, simulist$endingdate, mc.core=ncores)

final <- unlist(result)
