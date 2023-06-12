#Script estimating parameters for Samara model
#Authors : Florian Larue, Gregory Beurier, Lauriane Rouan -- (PAM, AGAP, BIOS, CIRAD)
####Adapted by Myriam ADAM (JUNE 2020) to run the optimisation
####on 23 sites in West Africa

####Install and load packages
list.of.packages <- c("rsamara", "RODBC", "parallel", "DEoptim")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)
library(plyr)
library("dplyr", lib.loc="~/R/win-library/3.4")
###added Myriam
library(tidyr)
library(data.table)
library (ggplot2)
####end #####

setwd("D:/Marksim")
connectDB('D:/Marksim/DATA_validation/Basedonnee_Malick_Myriam_july2020.accdb')
#Get data for simulation

#meteo1 <- loadMeteo('4', '2014/07/07', '2014/12/06')
meteoBBY2013 <- loadMeteo('380084', '2013/06/01', '2013/12/06')
meteoNIO2015 <- loadMeteo('380114', '2015/06/01', '2015/12/06')
meteoSINTH2013 <- loadMeteo('4', '2013/06/01', '2013/12/06')
meteoSINTH2014 <- loadMeteo('4', '2014/06/01', '2014/12/06')
meteoSINTH2015 <- loadMeteo('4', '2015/06/01', '2015/12/06')
meteoSINTH2016 <- loadMeteo('4', '2016/06/01', '2016/12/06')
meteoSMK2013 <- loadMeteo('3', '2013/06/01', '2013/12/06')
meteoSMK2014 <- loadMeteo('3', '2014/06/01', '2014/12/06')
meteoBama2014 <- loadMeteo('5', '2014/06/01', '2014/12/06')
meteoBama2015 <- loadMeteo('5', '2015/06/01', '2015/12/06')
meteoFRK2014 <- loadMeteo('2', '2014/06/01', '2014/12/06')

# meteo = rbind(meteoBBY2013, meteoNIO2015, 
#               meteoSINTH2013,meteoSINTH2014,meteoSINTH2015,meteoSINTH2016, 
#               meteoSMK2013, meteoSMK2014,
#               meteoBama2014, meteoBama2015, meteoFRK2014)
# meteo$wscode = as.factor(meteo$wscode)
# ggplot(data=meteo, aes(x=weatherdate, y=tmin, color=wscode )) +
#   geom_line() +  
#   geom_line(aes(x=weatherdate, y=tmax,color=wscode))
# ggplot(data=meteo, aes(x=weatherdate, y=rainfall, color=wscode )) +
#   geom_bar(stat="identity", position=position_dodge())+facet_wrap(~wscode)

#####end####


### get parameters and observations for all sites and varieties
list_variete=  data.frame("variete" =  c("Fadda", "621B", "Soumba", "IS15401",
                                         "CSM63E", "Local", "Grinkan", "Nieleni", "Pablo", "SK5912"))


paramInit_BAMA2014S1 = list()
vObs_BAMA2014S1  = list()
result_BAMA2014S1  = list()
obsRed_BAMA2014S1  = list()
resRed_BAMA2014S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_BAMA2014S1[[var]] <- loadSimDetails('BAMA2014S1', var, 'BAM_SOL', '5', '2014/06/01', '2014/12/06')
  result_BAMA2014S1[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
  
  if (var == "Fadda")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BAMA2014S1[[var]]<- loadObs("BAMA2014S1G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS15401" )
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BAMA2014S1[[var]]<- loadObs("BAMA2014S1G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_BAMA2014S1[[var]] <- loadObs("BAMA2014S1G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_BAMA2014S1[[var]]<- loadObs("BAMA2014S1G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_BAMA2014S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_BAMA2014S1[[var]], result_BAMA2014S1[[var]])
  resRed_BAMA2014S1[[var]] <- rsamara::rcpp_reduceResults(result_BAMA2014S1[[var]], vObs_BAMA2014S1[[var]])
  
  }



paramInit_BAMA2014S2 = list()
vObs_BAMA2014S2  = list()
result_BAMA2014S2  = list()
obsRed_BAMA2014S2  = list()
resRed_BAMA2014S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_BAMA2014S2[[var]] <- loadSimDetails('BAMA2014S2', var, 'BAM_SOL', '5', '2014/06/01', '2014/12/06')
  result_BAMA2014S2[[var]] <- rsamara::run2DF(paramInit_BAMA2014S2[[var]],meteoBama2014)
  
  if (var == "Fadda")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BAMA2014S2[[var]]<- loadObs("BAMA2014S2G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS15401" )
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BAMA2014S2[[var]]<- loadObs("BAMA2014S2G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_BAMA2014S2[[var]] <- loadObs("BAMA2014S2G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_BAMA2014S2[[var]]<- loadObs("BAMA2014S2G10", var,'2014/06/01', '2014/12/06')
  }
  
  obsRed_BAMA2014S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_BAMA2014S2[[var]], result_BAMA2014S2[[var]])
  resRed_BAMA2014S2[[var]] <- rsamara::rcpp_reduceResults(result_BAMA2014S2[[var]], vObs_BAMA2014S2[[var]])
  
}


paramInit_BAMA2014S3= list() 
vObs_BAMA2014S3  = list()
result_BAMA2014S3  = list()
obsRed_BAMA2014S3  = list()
resRed_BAMA2014S3  = list()

for (var in unique(list_variete$variete))
{
  paramInit_BAMA2014S3[[var]] <- loadSimDetails('BAMA2014S3', var, 'BAM_SOL', '5', '2014/06/01', '2014/12/06')
  result_BAMA2014S3[[var]] <- rsamara::run2DF(paramInit_BAMA2014S3[[var]],meteoBama2014)
  
  if (var == "Fadda")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BAMA2014S3[[var]]<- loadObs("BAMA2014S3G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS15401" )
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BAMA2014S3[[var]]<- loadObs("BAMA2014S3G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_BAMA2014S3[[var]] <- loadObs("BAMA2014S3G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_BAMA2014S3[[var]]<- loadObs("BAMA2014S3G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_BAMA2014S3[[var]] <- rsamara::rcpp_reduceVobs(vObs_BAMA2014S3[[var]], result_BAMA2014S3[[var]])
  resRed_BAMA2014S3[[var]] <- rsamara::rcpp_reduceResults(result_BAMA2014S3[[var]], vObs_BAMA2014S3[[var]])
  
  }




paramInit_BAMA2015S1 = list()
vObs_BAMA2015S1  = list()
result_BAMA2015S1  = list()
obsRed_BAMA2015S1  = list()
resRed_BAMA2015S1  = list()


for (var in unique(list_variete$variete))
{
  paramInit_BAMA2015S1[[var]] <- loadSimDetails('BAMA2015S1', var, 'BAM_SOL', '5', '2015/06/01', '2015/12/06')
  result_BAMA2015S1[[var]] <- rsamara::run2DF(paramInit_BAMA2015S1[[var]],meteoBama2015)
  
  if (var == "Fadda")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G1", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BAMA2015S1[[var]]<- loadObs("BAMA2015S1G2", var,'2015/06/01', '2015/12/06')
  } else if ( var=="IS15401" )
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G3", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BAMA2015S1[[var]]<- loadObs("BAMA2015S1G4", var,'2015/06/01', '2015/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G5", var,'2015/06/01', '2015/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G6", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G7", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G8", var,'2015/06/01', '2015/12/06')
  } else if ( var =="621B")
  {
    vObs_BAMA2015S1[[var]] <- loadObs("BAMA2015S1G9", var,'2015/06/01', '2015/12/06')
  } else 
  {
    vObs_BAMA2015S1[[var]]<- loadObs("BAMA2015S1G10", var,'2015/06/01', '2015/12/06')
  }
    
  obsRed_BAMA2015S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_BAMA2015S1[[var]], result_BAMA2015S1[[var]])
  resRed_BAMA2015S1[[var]] <- rsamara::rcpp_reduceResults(result_BAMA2015S1[[var]], vObs_BAMA2015S1[[var]])
  
  }
 
paramInit_BBY2013S1 = list()
vObs_BBY2013S1  = list()
result_BBY2013S1  = list()
obsRed_BBY2013S1  = list()
resRed_BBY2013S1  = list()
var = 'Fadda'
for (var in unique(list_variete$variete))
{
 paramInit_BBY2013S1[[var]] <- loadSimDetails('BBY2013S1', var, 'BBY_SOL', '380084', '2013/06/01', '2013/12/06')
  result_BBY2013S1[[var]] <- rsamara::run2DF(paramInit_BBY2013S1[[var]],meteoBBY2013)

  if (var == "Fadda")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BBY2013S1[[var]]<- loadObs("BBY2013S1G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS15401" )
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BBY2013S1[[var]]<- loadObs("BBY2013S1G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_BBY2013S1[[var]] <- loadObs("BBY2013S1G9", var,'2013/06/01', '2013/12/06')
  } else
  {
    vObs_BBY2013S1[[var]]<- loadObs("BBY2013S1G10", var,'2013/06/01', '2013/12/06')
  }

  obsRed_BBY2013S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_BBY2013S1[[var]], result_BBY2013S1[[var]])
  resRed_BBY2013S1[[var]] <- rsamara::rcpp_reduceResults(result_BBY2013S1[[var]], vObs_BBY2013S1[[var]])

  }

  
paramInit_BBY2013S2 = list()
vObs_BBY2013S2  = list()
result_BBY2013S2  = list()
obsRed_BBY2013S2  = list()
resRed_BBY2013S2  = list()


for (var in unique(list_variete$variete))
{
  paramInit_BBY2013S2[[var]] <- loadSimDetails('BBY2013S2', var, 'BBY_SOL', '380084', '2013/06/01', '2013/12/06')
  result_BBY2013S2[[var]] <- rsamara::run2DF(paramInit_BBY2013S2[[var]],meteoBBY2013)
  
  if (var == "Fadda")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_BBY2013S2[[var]]<- loadObs("BBY2013S2G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS25401" )
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_BBY2013S2[[var]]<- loadObs("BBY2013S2G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_BBY2013S2[[var]] <- loadObs("BBY2013S2G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_BBY2013S2[[var]]<- loadObs("BBY2013S2G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_BBY2013S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_BBY2013S2[[var]], result_BBY2013S2[[var]])
  resRed_BBY2013S2[[var]] <- rsamara::rcpp_reduceResults(result_BBY2013S2[[var]], vObs_BBY2013S2[[var]])
  
  }
  

paramInit_FRK2014S1 = list()
vObs_FRK2014S1  = list()
result_FRK2014S1  = list()
obsRed_FRK2014S1  = list()
resRed_FRK2014S1  = list()
# var = 'Fadda'
for (var in unique(list_variete$variete))
{
  paramInit_FRK2014S1[[var]] <- loadSimDetails('FRK2014S1', var, 'FRK_SOL', '2', '2014/06/01', '2014/12/06')
  result_FRK2014S1[[var]] <- rsamara::run2DF(paramInit_FRK2014S1[[var]],meteoFRK2014)

  if (var == "Fadda")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_FRK2014S1[[var]]<- loadObs("FRK2014S1G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS25401" )
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_FRK2014S1[[var]]<- loadObs("FRK2014S1G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_FRK2014S1[[var]] <- loadObs("FRK2014S1G9", var,'2014/06/01', '2014/12/06')
  } else
  {
    vObs_FRK2014S1[[var]]<- loadObs("FRK2014S1G10", var,'2014/06/01', '2014/12/06')
  }

  obsRed_FRK2014S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_FRK2014S1[[var]], result_FRK2014S1[[var]])
  resRed_FRK2014S1[[var]] <- rsamara::rcpp_reduceResults(result_FRK2014S1[[var]], vObs_FRK2014S1[[var]])

  }

# paramInitFRK2014S2= list()
paramInit_FRK2014S2 = list()
vObs_FRK2014S2  = list()
result_FRK2014S2  = list()
obsRed_FRK2014S2  = list()
resRed_FRK2014S2  = list()
# var = 'Fadda'
for (var in unique(list_variete$variete))
{
  paramInit_FRK2014S2[[var]] <- loadSimDetails('FRK2014S2', var, 'FRK_SOL', '2', '2014/06/01', '2014/12/06')
  result_FRK2014S2[[var]] <- rsamara::run2DF(paramInit_FRK2014S2[[var]],meteoFRK2014)

  if (var == "Fadda")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_FRK2014S2[[var]]<- loadObs("FRK2014S2G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS25401" )
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_FRK2014S2[[var]]<- loadObs("FRK2014S2G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_FRK2014S2[[var]] <- loadObs("FRK2014S2G9", var,'2014/06/01', '2014/12/06')
  } else
  {
    vObs_FRK2014S2[[var]]<- loadObs("FRK2014S2G10", var,'2014/06/01', '2014/12/06')
  }

  obsRed_FRK2014S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_FRK2014S2[[var]], result_FRK2014S2[[var]])
  resRed_FRK2014S2[[var]] <- rsamara::rcpp_reduceResults(result_FRK2014S2[[var]], vObs_FRK2014S2[[var]])

  }


# paramInitFRK2014S3= list()
paramInit_FRK2014S3 = list()
vObs_FRK2014S3  = list()
result_FRK2014S3  = list()
obsRed_FRK2014S3  = list()
resRed_FRK2014S3  = list()
# var = 'Fadda'
# 
for (var in unique(list_variete$variete))
{
  paramInit_FRK2014S3[[var]] <- loadSimDetails('FRK2014S3', var, 'FRK_SOL', '2', '2014/06/01', '2014/12/15')
  result_FRK2014S3[[var]] <- rsamara::run2DF(paramInit_FRK2014S3[[var]],meteoFRK2014)
  
  if (var == "Fadda")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G1", var,'2014/06/01', '2014/12/15')
  } else if ( var == "Nieleni")
  {
    vObs_FRK2014S3[[var]]<- loadObs("FRK2014S3G2", var,'2014/06/01', '2014/12/15')
  } else if ( var=="IS25401" )
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G3", var,'2014/06/01', '2014/12/15')
  } else if ( var == "Pablo")
  {
    vObs_FRK2014S3[[var]]<- loadObs("FRK2014S3G4", var,'2014/06/01', '2014/12/15')
  } else if ( var == "CSM63E")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G5", var,'2014/06/01', '2014/12/15')
  } else if ( var == "SK5912")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G6", var,'2014/06/01', '2014/12/15')
  } else if ( var == "Grinkan")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G8", var,'2014/06/01', '2014/12/15')
  } else if ( var =="621B")
  {
    vObs_FRK2014S3[[var]] <- loadObs("FRK2014S3G9", var,'2014/06/01', '2014/12/15')
  } else
  {
    vObs_FRK2014S3[[var]]<- loadObs("FRK2014S3G10", var,'2014/06/01', '2014/12/15')
  }
  
  obsRed_FRK2014S3[[var]] <- rsamara::rcpp_reduceVobs(vObs_FRK2014S3[[var]], result_FRK2014S3[[var]])
  resRed_FRK2014S3[[var]] <- rsamara::rcpp_reduceResults(result_FRK2014S3[[var]], vObs_FRK2014S3[[var]])
  
}


# paramInitNIO2015S1= list() 
paramInit_NIO2015S1 = list()
vObs_NIO2015S1  = list()
result_NIO2015S1  = list()
obsRed_NIO2015S1  = list()
resRed_NIO2015S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_NIO2015S1[[var]] <- loadSimDetails('NIO2015S1', var, 'NIO1_SOL', '380114', '2015/06/01', '2015/12/06')
  result_NIO2015S1[[var]] <- rsamara::run2DF(paramInit_NIO2015S1[[var]],meteoNIO2015)
  
  if (var == "Fadda")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G1", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_NIO2015S1[[var]]<- loadObs("NIO2015S1G2", var,'2015/06/01', '2015/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G3", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Pablo")
  {
    vObs_NIO2015S1[[var]]<- loadObs("NIO2015S1G4", var,'2015/06/01', '2015/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G5", var,'2015/06/01', '2015/12/06')
  } else if ( var == "SK5912")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G6", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G7", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Soumba")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G8", var,'2015/06/01', '2015/12/06')
  } else if ( var =="621B")
  {
    vObs_NIO2015S1[[var]] <- loadObs("NIO2015S1G9", var,'2015/06/01', '2015/12/06')
  } else 
  {
    vObs_NIO2015S1[[var]]<- loadObs("NIO2015S1G10", var,'2015/06/01', '2015/12/06')
  }
    
  obsRed_NIO2015S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_NIO2015S1[[var]], result_NIO2015S1[[var]])
  resRed_NIO2015S1[[var]] <- rsamara::rcpp_reduceResults(result_NIO2015S1[[var]], vObs_NIO2015S1[[var]])
  
  }
  
# paramInitNIO2015S2= list() 
paramInit_NIO2015S2 = list()
vObs_NIO2015S2  = list()
result_NIO2015S2  = list()
obsRed_NIO2015S2  = list()
resRed_NIO2015S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_NIO2015S2[[var]] <- loadSimDetails('NIO2015S2', var, 'NIO1_SOL', '380114', '2015/06/01', '2015/12/06')
  result_NIO2015S2[[var]] <- rsamara::run2DF(paramInit_NIO2015S2[[var]],meteoNIO2015)
  
  if (var == "Fadda")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G1", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_NIO2015S2[[var]]<- loadObs("NIO2015S2G2", var,'2015/06/01', '2015/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G3", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Pablo")
  {
    vObs_NIO2015S2[[var]]<- loadObs("NIO2015S2G4", var,'2015/06/01', '2015/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G5", var,'2015/06/01', '2015/12/06')
  } else if ( var == "SK5912")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G6", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G7", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Soumba")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G8", var,'2015/06/01', '2015/12/06')
  } else if ( var =="621B")
  {
    vObs_NIO2015S2[[var]] <- loadObs("NIO2015S2G9", var,'2015/06/01', '2015/12/06')
  } else 
  {
    vObs_NIO2015S2[[var]]<- loadObs("NIO2015S2G10", var,'2015/06/01', '2015/12/06')
  }
    
  obsRed_NIO2015S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_NIO2015S2[[var]], result_NIO2015S2[[var]])
  resRed_NIO2015S2[[var]] <- rsamara::rcpp_reduceResults(result_NIO2015S2[[var]], vObs_NIO2015S2[[var]])
  
  }




# paramInitSINT2013S1= list() 
paramInit_SINT2013S1 = list()
vObs_SINT2013S1  = list()
result_SINT2013S1  = list()
obsRed_SINT2013S1  = list()
resRed_SINT2013S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2013S1[[var]] <- loadSimDetails('SINT2013S1', var, 'SIN1_SOL', '4', '2013/06/01', '2013/12/06')
  result_SINT2013S1[[var]] <- rsamara::run2DF(paramInit_SINT2013S1[[var]],meteoSINTH2013)
  
  if (var == "Fadda")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2013S1[[var]]<- loadObs("SINT2013S1G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2013S1[[var]]<- loadObs("SINT2013S1G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2013S1[[var]] <- loadObs("SINT2013S1G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_SINT2013S1[[var]]<- loadObs("SINT2013S1G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_SINT2013S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2013S1[[var]], result_SINT2013S1[[var]])
  resRed_SINT2013S1[[var]] <- rsamara::rcpp_reduceResults(result_SINT2013S1[[var]], vObs_SINT2013S1[[var]])
  
  }

# paramInitSINT2013S2= list() 
paramInit_SINT2013S2 = list()
vObs_SINT2013S2  = list()
result_SINT2013S2  = list()
obsRed_SINT2013S2  = list()
resRed_SINT2013S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2013S2[[var]] <- loadSimDetails('SINT2013S2', var, 'SIN1_SOL', '4', '2013/06/01', '2013/12/06')
  result_SINT2013S2[[var]] <- rsamara::run2DF(paramInit_SINT2013S2[[var]],meteoSINTH2013)
  
  if (var == "Fadda")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2013S2[[var]]<- loadObs("SINT2013S2G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2013S2[[var]]<- loadObs("SINT2013S2G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2013S2[[var]] <- loadObs("SINT2013S2G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_SINT2013S2[[var]]<- loadObs("SINT2013S2G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_SINT2013S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2013S2[[var]], result_SINT2013S2[[var]])
  resRed_SINT2013S2[[var]] <- rsamara::rcpp_reduceResults(result_SINT2013S2[[var]], vObs_SINT2013S2[[var]])
  
  }

# paramInitSINT2014S1= list() 
paramInit_SINT2014S1 = list()
vObs_SINT2014S1  = list()
result_SINT2014S1  = list()
obsRed_SINT2014S1  = list()
resRed_SINT2014S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2014S1[[var]] <- loadSimDetails('SINT2014S1', var, 'SIN1_SOL', '4', '2014/06/01', '2014/12/06')
  result_SINT2014S1[[var]] <- rsamara::run2DF(paramInit_SINT2014S1[[var]],meteoSINTH2014)
  
  if (var == "Fadda")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2014S1[[var]]<- loadObs("SINT2014S1G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2014S1[[var]]<- loadObs("SINT2014S1G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2014S1[[var]] <- loadObs("SINT2014S1G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_SINT2014S1[[var]]<- loadObs("SINT2014S1G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_SINT2014S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2014S1[[var]], result_SINT2014S1[[var]])
  resRed_SINT2014S1[[var]] <- rsamara::rcpp_reduceResults(result_SINT2014S1[[var]], vObs_SINT2014S1[[var]])
  
  }

# paramInitSINT2014S2= list() 
paramInit_SINT2014S2 = list()
vObs_SINT2014S2  = list()
result_SINT2014S2  = list()
obsRed_SINT2014S2  = list()
resRed_SINT2014S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2014S2[[var]] <- loadSimDetails('SINT2014S2', var, 'SIN1_SOL', '4', '2014/06/01', '2014/12/06')
  result_SINT2014S2[[var]] <- rsamara::run2DF(paramInit_SINT2014S2[[var]],meteoSINTH2014)
  
  if (var == "Fadda")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2014S2[[var]]<- loadObs("SINT2014S2G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2014S2[[var]]<- loadObs("SINT2014S2G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2014S2[[var]] <- loadObs("SINT2014S2G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_SINT2014S2[[var]]<- loadObs("SINT2014S2G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_SINT2014S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2014S2[[var]], result_SINT2014S2[[var]])
  resRed_SINT2014S2[[var]] <- rsamara::rcpp_reduceResults(result_SINT2014S2[[var]], vObs_SINT2014S2[[var]])
  
  }

# paramInitSINT2015S1= list() 
paramInit_SINT2015S1 = list()
vObs_SINT2015S1  = list()
result_SINT2015S1  = list()
obsRed_SINT2015S1  = list()
resRed_SINT2015S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2015S1[[var]] <- loadSimDetails('SINT2015S1', var, 'SIN1_SOL', '4', '2015/06/01', '2015/12/06')
  result_SINT2015S1[[var]] <- rsamara::run2DF(paramInit_SINT2015S1[[var]],meteoSINTH2015)
  
  if (var == "Fadda")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G1", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2015S1[[var]]<- loadObs("SINT2015S1G2", var,'2015/06/01', '2015/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G3", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2015S1[[var]]<- loadObs("SINT2015S1G4", var,'2015/06/01', '2015/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G5", var,'2015/06/01', '2015/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G6", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G7", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G8", var,'2015/06/01', '2015/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2015S1[[var]] <- loadObs("SINT2015S1G9", var,'2015/06/01', '2015/12/06')
  } else 
  {
    vObs_SINT2015S1[[var]]<- loadObs("SINT2015S1G10", var,'2015/06/01', '2015/12/06')
  }
    
  obsRed_SINT2015S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2015S1[[var]], result_SINT2015S1[[var]])
  resRed_SINT2015S1[[var]] <- rsamara::rcpp_reduceResults(result_SINT2015S1[[var]], vObs_SINT2015S1[[var]])
  
  }

# paramInitSINT2015S2= list() 
paramInit_SINT2015S2 = list()
vObs_SINT2015S2  = list()
result_SINT2015S2  = list()
obsRed_SINT2015S2  = list()
resRed_SINT2015S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2015S2[[var]] <- loadSimDetails('SINT2015S2', var, 'SIN1_SOL', '4', '2015/06/01', '2015/12/06')
  result_SINT2015S2[[var]] <- rsamara::run2DF(paramInit_SINT2015S2[[var]],meteoSINTH2015)
  
  if (var == "Fadda")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G1", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2015S2[[var]]<- loadObs("SINT2015S2G2", var,'2015/06/01', '2015/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G3", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2015S2[[var]]<- loadObs("SINT2015S2G4", var,'2015/06/01', '2015/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G5", var,'2015/06/01', '2015/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G6", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G7", var,'2015/06/01', '2015/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G8", var,'2015/06/01', '2015/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2015S2[[var]] <- loadObs("SINT2015S2G9", var,'2015/06/01', '2015/12/06')
  } else 
  {
    vObs_SINT2015S2[[var]]<- loadObs("SINT2015S2G10", var,'2015/06/01', '2015/12/06')
  }
    
  obsRed_SINT2015S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2015S2[[var]], result_SINT2015S2[[var]])
  resRed_SINT2015S2[[var]] <- rsamara::rcpp_reduceResults(result_SINT2015S2[[var]], vObs_SINT2015S2[[var]])
  
  }

# paramInitSINT2016S1= list() 
paramInit_SINT2016S1 = list()
vObs_SINT2016S1  = list()
result_SINT2016S1  = list()
obsRed_SINT2016S1  = list()
resRed_SINT2016S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SINT2016S1[[var]] <- loadSimDetails('SINT2016S1', var, 'SIN1_SOL', '4', '2016/06/01', '2016/12/06')
  result_SINT2016S1[[var]] <- rsamara::run2DF(paramInit_SINT2016S1[[var]],meteoSINTH2016)
  
  if (var == "Fadda")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G1", var,'2016/06/01', '2016/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SINT2016S1[[var]]<- loadObs("SINT2016S1G2", var,'2016/06/01', '2016/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G3", var,'2016/06/01', '2016/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SINT2016S1[[var]]<- loadObs("SINT2016S1G4", var,'2016/06/01', '2016/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G5", var,'2016/06/01', '2016/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G6", var,'2016/06/01', '2016/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G7", var,'2016/06/01', '2016/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G8", var,'2016/06/01', '2016/12/06')
  } else if ( var =="621B")
  {
    vObs_SINT2016S1[[var]] <- loadObs("SINT2016S1G9", var,'2016/06/01', '2016/12/06')
  } else 
  {
    vObs_SINT2016S1[[var]]<- loadObs("SINT2016S1G10", var,'2016/06/01', '2016/12/06')
  }
    
  obsRed_SINT2016S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SINT2016S1[[var]], result_SINT2016S1[[var]])
  resRed_SINT2016S1[[var]] <- rsamara::rcpp_reduceResults(result_SINT2016S1[[var]], vObs_SINT2016S1[[var]])
  
  }

# paramInitSMK2013S1= list() 
paramInit_SMK2013S1 = list()
vObs_SMK2013S1  = list()
result_SMK2013S1  = list()
obsRed_SMK2013S1  = list()
resRed_SMK2013S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SMK2013S1[[var]] <- loadSimDetails('SMK2013S1', var, 'SMK_SOL', '3', '2013/06/01', '2013/12/06')
  result_SMK2013S1[[var]] <- rsamara::run2DF(paramInit_SMK2013S1[[var]],meteoSMK2013)
  
  if (var == "Fadda")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SMK2013S1[[var]]<- loadObs("SMK2013S1G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SMK2013S1[[var]]<- loadObs("SMK2013S1G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_SMK2013S1[[var]] <- loadObs("SMK2013S1G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_SMK2013S1[[var]]<- loadObs("SMK2013S1G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_SMK2013S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SMK2013S1[[var]], result_SMK2013S1[[var]])
  resRed_SMK2013S1[[var]] <- rsamara::rcpp_reduceResults(result_SMK2013S1[[var]], vObs_SMK2013S1[[var]])
  
  }
  
  
# paramInitSMK2013S2= list() 
paramInit_SMK2013S2 = list()
vObs_SMK2013S2  = list()
result_SMK2013S2  = list()
obsRed_SMK2013S2  = list()
resRed_SMK2013S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SMK2013S2[[var]] <- loadSimDetails('SMK2013S2', var, 'SMK_SOL', '3', '2013/06/01', '2013/12/06')
  result_SMK2013S2[[var]] <- rsamara::run2DF(paramInit_SMK2013S2[[var]],meteoSMK2013)
  
  if (var == "Fadda")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SMK2013S2[[var]]<- loadObs("SMK2013S2G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SMK2013S2[[var]]<- loadObs("SMK2013S2G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_SMK2013S2[[var]] <- loadObs("SMK2013S2G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_SMK2013S2[[var]]<- loadObs("SMK2013S2G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_SMK2013S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_SMK2013S2[[var]], result_SMK2013S2[[var]])
  resRed_SMK2013S2[[var]] <- rsamara::rcpp_reduceResults(result_SMK2013S2[[var]], vObs_SMK2013S2[[var]])
  
  }
  
  
# paramInitSMK2013S3= list()
paramInit_SMK2013S3 = list()
vObs_SMK2013S3  = list()
result_SMK2013S3  = list()
obsRed_SMK2013S3  = list()
resRed_SMK2013S3  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SMK2013S3[[var]] <- loadSimDetails('SMK2013S3', var, 'SMK_SOL', '3', '2013/06/01', '2013/12/06')
  result_SMK2013S3[[var]] <- rsamara::run2DF(paramInit_SMK2013S3[[var]],meteoSMK2013)
  
  if (var == "Fadda")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G1", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SMK2013S3[[var]]<- loadObs("SMK2013S3G2", var,'2013/06/01', '2013/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G3", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SMK2013S3[[var]]<- loadObs("SMK2013S3G4", var,'2013/06/01', '2013/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G5", var,'2013/06/01', '2013/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G6", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G7", var,'2013/06/01', '2013/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G8", var,'2013/06/01', '2013/12/06')
  } else if ( var =="621B")
  {
    vObs_SMK2013S3[[var]] <- loadObs("SMK2013S3G9", var,'2013/06/01', '2013/12/06')
  } else 
  {
    vObs_SMK2013S3[[var]]<- loadObs("SMK2013S3G10", var,'2013/06/01', '2013/12/06')
  }
    
  obsRed_SMK2013S3[[var]] <- rsamara::rcpp_reduceVobs(vObs_SMK2013S3[[var]], result_SMK2013S3[[var]])
  resRed_SMK2013S3[[var]] <- rsamara::rcpp_reduceResults(result_SMK2013S3[[var]], vObs_SMK2013S3[[var]])
  
  }
  
# paramInitSMK2014S1= list() 
paramInit_SMK2014S1 = list()
vObs_SMK2014S1  = list()
result_SMK2014S1  = list()
obsRed_SMK2014S1  = list()
resRed_SMK2014S1  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SMK2014S1[[var]] <- loadSimDetails('SMK2014S1', var, 'SMK_SOL', '3', '2014/06/01', '2014/12/06')
  result_SMK2014S1[[var]] <- rsamara::run2DF(paramInit_SMK2014S1[[var]],meteoSMK2014)
  
  if (var == "Fadda")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SMK2014S1[[var]]<- loadObs("SMK2014S1G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SMK2014S1[[var]]<- loadObs("SMK2014S1G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_SMK2014S1[[var]] <- loadObs("SMK2014S1G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_SMK2014S1[[var]]<- loadObs("SMK2014S1G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_SMK2014S1[[var]] <- rsamara::rcpp_reduceVobs(vObs_SMK2014S1[[var]], result_SMK2014S1[[var]])
  resRed_SMK2014S1[[var]] <- rsamara::rcpp_reduceResults(result_SMK2014S1[[var]], vObs_SMK2014S1[[var]])
  
  }
  
  
# paramInitSMK2014S2= list() 
paramInit_SMK2014S2 = list()
vObs_SMK2014S2  = list()
result_SMK2014S2  = list()
obsRed_SMK2014S2  = list()
resRed_SMK2014S2  = list()

for (var in unique(list_variete$variete))
{
  paramInit_SMK2014S2[[var]] <- loadSimDetails('SMK2014S2', var, 'SMK_SOL', '3', '2014/06/01', '2014/12/06')
  result_SMK2014S2[[var]] <- rsamara::run2DF(paramInit_SMK2014S2[[var]],meteoSMK2014)
  
  if (var == "Fadda")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G1", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Nieleni")
  {
    vObs_SMK2014S2[[var]]<- loadObs("SMK2014S2G2", var,'2014/06/01', '2014/12/06')
  } else if ( var=="IS35401" )
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G3", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Pablo")
  {
    vObs_SMK2014S2[[var]]<- loadObs("SMK2014S2G4", var,'2014/06/01', '2014/12/06')
  } else if ( var == "CSM63E")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G5", var,'2014/06/01', '2014/12/06')
  } else if ( var == "SK5912")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G6", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Grinkan")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G7", var,'2014/06/01', '2014/12/06')
  } else if ( var == "Soumba")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G8", var,'2014/06/01', '2014/12/06')
  } else if ( var =="621B")
  {
    vObs_SMK2014S2[[var]] <- loadObs("SMK2014S2G9", var,'2014/06/01', '2014/12/06')
  } else 
  {
    vObs_SMK2014S2[[var]]<- loadObs("SMK2014S2G10", var,'2014/06/01', '2014/12/06')
  }
    
  obsRed_SMK2014S2[[var]] <- rsamara::rcpp_reduceVobs(vObs_SMK2014S2[[var]], result_SMK2014S2[[var]])
  resRed_SMK2014S2[[var]] <- rsamara::rcpp_reduceResults(result_SMK2014S2[[var]], vObs_SMK2014S2[[var]])
  
  }
 
odbcCloseAll()
#####end####

list_trial=  data.frame ("trial" = c("BAMA2014S1","BAMA2014S2","BAMA2014S3","BAMA2015S1",
                                     #"BBY2013S1",
                                     "BBY2013S2",
                                     #"FRK2014S1","FRK2014S2","FRK2014S3",
                                     "NIO2015S1","NIO2015S2",
                                     "SINT2013S1","SINT2013S2","SINT2014S1","SINT2014S2","SINT2015S1","SINT2015S2","SINT2016S1",
                                     "SMK2013S1","SMK2013S2","SMK2013S3","SMK2014S1","SMK2014S2"))

####optimisation 
ParamOfInterest <- c("rootfrontmax","pfactor","kcmax")
MinValue <- c(500,0.5,1)
MaxValue <- c(2000,1,1.5)
obsCoef <- c(1500,0.6,1.15)
MaxIter <- 500
SolTol <- 0.05
NbParam <- length(ParamOfInterest)
Bounds <- matrix(c(MinValue,MaxValue),ncol=2)

 
Optim_Samara_funct <- function(p){
  
  for (trial in unique(list_trial$trial))
  {
    paramInit= paste0("paramInit_",trial,"_var")
    paramInitTrans<- get(paramInit)
    setkey(paramInitTrans)
    assign(paste0("paramInitTrans_",trial),paramInitTrans)
    
    p <<-p
    paramInitTrans[ParamOfInterest] <- p
    parameters<-  paramInitTrans
    assign(paste0("paramInitTrans_",trial),paramInitTrans)
    assign(paste0("parameters_",trial),parameters)
    
  }

    result_BAMA2014S1 <- rsamara::run2DF(parameters_BAMA2014S1[,2:216],meteoBama2014)
    result_BAMA2014S2 <- rsamara::run2DF(parameters_BAMA2014S2[,2:216],meteoBama2014)
    result_BAMA2014S3 <- rsamara::run2DF(parameters_BAMA2014S3[,2:216],meteoBama2014)
    result_BAMA2015S1 <- rsamara::run2DF(parameters_BAMA2015S1[,2:216],meteoBama2015)

    result_BBY2013S1[[var]] <- rsamara::run2DF(parameters_BBY2013S1[,2:216],meteoBBY2013)
    result_BBY2013S2 <- rsamara::run2DF(parameters_BBY2013S2[,2:216],meteoBBY2013)

   result_FRK2014S1[[var]] <- rsamara::run2DF(parameters_FRK2014S1[,2:216],meteoFRK2014)
    result_FRK2014S2[[var]] <- rsamara::run2DF(parameters_FRK2014S2[,2:216],meteoFRK2014)
    result_FRK2014S3[[var]] <- rsamara::run2DF(parameters_FRK2014S3[,2:216],meteoFRK2014)

    result_NIO2015S1 <- rsamara::run2DF(parameters_NIO2015S1[,2:216],meteoNIO2015)
    result_NIO2015S2 <- rsamara::run2DF(parameters_NIO2015S2[,2:216],meteoNIO2015)

    result_SINT2013S1 <- rsamara::run2DF(parameters_SINT2013S1[,2:216],meteoSINTH2013)
    result_SINT2013S2 <- rsamara::run2DF(parameters_SINT2013S2[,2:216],meteoSINTH2013)
    result_SINT2014S1 <- rsamara::run2DF(parameters_SINT2014S1[,2:216],meteoSINTH2014)
    result_SINT2014S2 <- rsamara::run2DF(parameters_SINT2014S2[,2:216],meteoSINTH2014)
    result_SINT2015S1 <- rsamara::run2DF(parameters_SINT2015S1[,2:216],meteoSINTH2015)
    result_SINT2015S2<- rsamara::run2DF(parameters_SINT2015S2[,2:216],meteoSINTH2015)
    result_SINT2016S1 <- rsamara::run2DF(parameters_SINT2016S1[,2:216],meteoSINTH2016)


    result_SMK2013S1 <- rsamara::run2DF(parameters_SMK2013S1[,2:216],meteoSMK2013)
    result_SMK2013S2<- rsamara::run2DF(parameters_SMK2013S2[,2:216],meteoSMK2013)
    result_SMK2013S3 <- rsamara::run2DF(parameters_SMK2013S3[,2:216],meteoSMK2013)
    result_SMK2014S1 <- rsamara::run2DF(parameters_SMK2014S1[,2:216],meteoSMK2014)
    result_SMK2014S2 <- rsamara::run2DF(parameters_SMK2014S2[,2:216],meteoSMK2014)

    obs_grain= c()
    obs_biomass= c()
    
    sim_grain= c()
    sim_biomass= c()
    
    obs_sim_final = list()
 
    for (trial in unique(list_trial$trial))
    {
      obsRed= paste0("obsRed_",trial,"_var")
      simRed= paste0("result_",trial)
      obs_grain = max(get(obsRed)$grainyieldpopfin,na.rm=TRUE)
      obs_biomass= max(get(obsRed)$drymatabovegroundpopfin,na.rm=TRUE)
      sim_grain =  max(get(simRed)$GrainYieldPop,na.rm=TRUE)
      sim_biomass =max(get(simRed)$DryMatAboveGroundTotPop,na.rm=TRUE)
      
      og= data.frame(obs_grain)
      ob = data.frame(obs_biomass)
      names(og)= paste0("obs_grainyield")
      names(ob)= paste0("obs_biomassyield")
      sg= data.frame(sim_grain)
      sb = data.frame(sim_biomass)
      names(sg)= paste0("sim_grainyield")
      names(sb)= paste0("sim_biomassyield")
      t = cbind(ob,og,sb,sg)
      obs_sim_final[[trial]] = t
      }
    
   
    obs_sim_final = rbindlist(obs_sim_final,idcol = "trial")

 
    obs_sim_final$obs_grainyield[obs_sim_final$obs_grainyield == "-Inf"] <- 0
    obs_sim_final$difsq_grain_yield <- 
                                ( obs_sim_final$obs_grainyield -  
                                obs_sim_final$sim_grainyield)^2
   RMSE_grain_yield <-sqrt(sum(  obs_sim_final$difsq_grain_yield )/
                            length(  obs_sim_final$difsq_grain_yield ))
  rRMSE_grain_yield = RMSE_grain_yield/mean(obs_sim_final$obs_grainyield)
  
  obs_sim_final$obs_biomassyield[obs_sim_final$obs_biomassyield == "-Inf"] <- 0
  obs_sim_final$difsq_biomass <- 
    ( obs_sim_final$obs_biomassyield -  
        obs_sim_final$sim_biomassyield)^2
   RMSE_biomass <-sqrt(sum( obs_sim_final$difsq_biomass )/
                            length(  obs_sim_final$difsq_biomass ))
   rRMSE_biomass = RMSE_biomass/mean(obs_sim_final$obs_biomassyield)
  
  
  return ((rRMSE_grain_yield+rRMSE_biomass)/2)
 
}

###end###
#### create param table with every row being one variety
paramInit = c()
paramInit_list = list()

for (trial in unique(list_trial$trial)) {
  paramInit = paste0("paramInit_",trial)
  paramInit_table= rbindlist(get(paramInit),idcol="variety")
  paramInit_table$site = trial
  assign(paste0("paramInit_",trial,'_table'),
         paramInit_table)
  paramInit_list[[trial]] = paramInit_table
  
   obsRed = paste0("obsRed_",trial)
   obsRed_table  = rbindlist(get(obsRed),fill=TRUE,idcol=TRUE)
   assign( paste0("obsRed_",trial,'_table'),
           obsRed_table)
}

paramInit_final = rbindlist(paramInit_list,idcol = "trial")
write.table( paramInit_final, file = "param_all.txt")

Optim_list <-list()
plot_optim_list <-list()
for (var in unique(list_variete$variete))
{
  for (trial in unique(list_trial$trial)){
    
    paramInit_table = paste0("paramInit_",trial,'_table')
    paramInit_var = subset (get(paramInit_table), get(paramInit_table)$variety == var)
    assign(paste0("paramInit_",trial,'_var'),paramInit_var)
    
    obsRed_table = paste0("obsRed_",trial,'_table')
    obsRed_var  = subset (get(obsRed_table), get(obsRed_table)$.id == var)
    assign(paste0("obsRed_",trial,'_var'),obsRed_var )
    
  }
  resOptim <- DEoptim(Optim_Samara_funct, lower=Bounds[,1], upper=Bounds[,2],
                      DEoptim.control(VTR=SolTol,itermax=MaxIter, strategy=2,
                                      trace=max(1,MaxIter/20)))
  Optim_list[[var]] =  resOptim
  
  plot_optim = ggplot() +geom_line(aes(resOptim$member$bestmemit,resOptim$member$bestvalit))+
    geom_point(aes(resOptim$member$bestmemit,resOptim$member$bestvalit),
               color="red", size=3)+ ggtitle(var)
  
  plot_optim_list [[var]] =  plot_optim
  
}

list_param = list()
list_optim = list()
for (var in unique(list_variete$variete))
  {
    param_optim = Optim_list[[var]][["optim"]][["bestmem"]]
    rRMSE_optim = Optim_list[[var]][["optim"]][["bestval"]]
    variety= var
    rRMSE_it = Optim_list[[var]][["member"]][["bestvalit"]]
    param_it =  Optim_list[[var]][["member"]][["bestmemit"]]
    list_param[[var]] = as.data.frame (cbind(variety, 
                                             t(param_optim),rRMSE_optim ))
    list_optim[[var]] = as.data.frame (cbind(t(param_it),rRMSE_it ))
}
  
  final_param = rbindlist(list_param)
  final_val_it = rbindlist(list_optim, idcol = TRUE)
  final_param$rootfrontmax = as.numeric(as.character(final_param$par1))
  final_param$pfactor= as.numeric(as.character(final_param$par2))
  final_param$kcmax = as.numeric(as.character(final_param$par3))

  write.table(final_param, file = "param_WS.txt")
  write.table(final_val_it, file = "param_WS_it.txt")
  
  
  
  #####join the models results and observation in one file 
  ####for the 10 varieties and 23 sites
  library(data.table)
  list_variete=  data.frame("variete" =  c("Fadda", "621B", "Soumba", "IS15401",
                                           "CSM63E", "Local", "Grinkan",
                                           "Nieleni", "Pablo", "SK5912"))

  result_BAMA2014S1_all =   rbindlist(  result_BAMA2014S1, idcol = 'variete')   
  result_BAMA2014S2_all =   rbindlist(  result_BAMA2014S2, idcol = 'variete')        
  result_BAMA2014S3_all =   rbindlist(  result_BAMA2014S3 , idcol = 'variete')    
  result_BAMA2015S1_all =  rbindlist(   result_BAMA2015S1 , idcol = 'variete') 
  
  result_BBY2013S1_all =   rbindlist(   result_BBY2013S2  , idcol = 'variete')
  result_BBY2013S2_all =   rbindlist(   result_BBY2013S2  , idcol = 'variete')
  
  result_FRK2014S1_all =  rbindlist(   result_FRK2014S1 , idcol = 'variete') 
  result_FRK2014S2_all =   rbindlist(  result_FRK2014S2, idcol = 'variete')        
  result_FRK2014S3_all =   rbindlist(  result_FRK2014S3 , idcol = 'variete')    

  
  result_NIO2015S1_all =   rbindlist(   result_NIO2015S1  , idcol = 'variete')     
  result_NIO2015S2_all =   rbindlist(   result_NIO2015S2 , idcol = 'variete') 
  
  result_SINT2013S1_all =  rbindlist(   result_SINT2013S1 , idcol = 'variete')
  result_SINT2013S2_all =  rbindlist(   result_SINT2013S2 , idcol = 'variete')
  result_SINT2014S1_all =  rbindlist(   result_SINT2014S1 , idcol = 'variete')
  result_SINT2014S2_all =  rbindlist(   result_SINT2014S2 , idcol = 'variete')
  result_SINT2015S1_all =  rbindlist(   result_SINT2015S1 , idcol = 'variete')        
  result_SINT2015S2_all =  rbindlist(   result_SINT2015S2 , idcol = 'variete')         
  result_SINT2016S1_all =  rbindlist(   result_SINT2016S1 , idcol = 'variete')
  
  result_SMK2013S1_all =   rbindlist(   result_SMK2013S1 , idcol = 'variete')        
  result_SMK2013S2_all =   rbindlist(   result_SMK2013S2 , idcol = 'variete')           
  result_SMK2013S3_all =   rbindlist(   result_SMK2013S3 , idcol = 'variete')         
  result_SMK2014S1_all =   rbindlist(   result_SMK2014S1  , idcol = 'variete')        
  result_SMK2014S2_all =   rbindlist(   result_SMK2014S2 , idcol = 'variete')
  
  
  result_BAMA2014S1_all$site = "BAMA2014S1"
  result_BAMA2014S1_all$type = 'eval'
  result_BAMA2014S2_all$site = "BAMA2014S2"
  result_BAMA2014S2_all$type= 'eval'
  result_BAMA2014S3_all$site = "BAMA2014S3"
  result_BAMA2014S3_all$type= 'eval'
  result_BAMA2015S1_all$site = "BAMA2015S1"
  result_BAMA2015S1_all$type= 'eval'
  
  result_BBY2013S1_all$site = "BBY2013S1"
  result_BBY2013S1_all$type= 'eval'
  result_BBY2013S2_all$site = "BBY2013S2"
  result_BBY2013S2_all$type= 'eval'
  result_NIO2015S1_all$site = "NIO2015S1"
  result_NIO2015S1_all$type= 'eval'
  result_NIO2015S2_all$site = "NIO2015S2"
  result_NIO2015S2_all$type= 'valid'
    
    
  result_SINT2013S1_all$site = "SINT2013S1"
    result_SINT2013S1_all$type= 'valid'
    result_SINT2013S2_all$site = "SINT2013S2"
    result_SINT2013S2_all$type= 'valid'
  result_SINT2014S1_all$site = "SINT2014S1"
    result_SINT2014S1_all$type= 'calib'
  result_SINT2014S2_all$site = "SINT2014S2"
    result_SINT2014S2_all$type= 'calib'
  result_SINT2015S1_all$site = "SINT2015S1"
    result_SINT2015S1_all$type= 'eval'
  result_SINT2015S2_all$site = "SINT2015S2"
    result_SINT2015S2_all$type= 'valid'
  result_SINT2016S1_all$site = "SINT2016S1"
    result_SINT2016S1_all$type= 'calib'
      
      
  result_SMK2013S1_all$site = "SMK2013S1"
  result_SMK2013S1_all$type= 'eval'
  result_SMK2013S2_all$site = "SMK2013S2"
  result_SMK2013S2_all$type= 'eval'
  result_SMK2013S3_all$site = "SMK2013S3"
  result_SMK2013S3_all$type= 'eval'
  result_SMK2014S1_all$site = "SMK2014S1"
  result_SMK2014S1_all$type= 'eval'
  result_SMK2014S2_all$site = "SMK2014S2"
  result_SMK2014S2_all$type= 'eval'
  
  
  result_FRK2014S1_all$site = "FRK2014S1"
  result_FRK2014S1_all$type = 'eval'
  result_FRK2014S2_all$site = "FRK2014S2"
  result_FRK2014S2_all$type= 'eval'
  result_FRK2014S3_all$site = "FRK2014S3"
  result_FRK2014S3_all$type= 'eval'
  
  
  results_all = rbind(   result_BAMA2014S1_all,
                         result_BAMA2014S2_all,
                         result_BAMA2014S3_all,
                         result_BAMA2015S1_all,
                         
                         result_BBY2013S1_all, 
                         result_BBY2013S2_all, 
                         
                         result_FRK2014S1_all,
                         result_FRK2014S2_all,
                         result_FRK2014S3_all,
                         
                         result_NIO2015S1_all, 
                         result_NIO2015S2_all, 
                         
                         result_SINT2013S1_all,
                         result_SINT2013S2_all,
                         result_SINT2014S1_all,
                         result_SINT2014S2_all,
                         result_SINT2015S1_all,
                         result_SINT2015S2_all,
                         result_SINT2016S1_all,
                         
                         result_SMK2013S1_all, 
                         result_SMK2013S2_all, 
                         result_SMK2013S3_all, 
                         result_SMK2014S1_all,
                         result_SMK2014S2_all) 
  
  write.table(results_all, file = 'D:/Marksim/Analyse_august2020/results_run_calib_malick_allsites3.txt')
 # write.table(results_all, file = 'D:/Marksim/Analyse_august2020/results_run_calib_malick_allsites2.txt')
  
  obsRed_BAMA2014S1_all =   rbindlist(  obsRed_BAMA2014S1, fill = TRUE,idcol = 'variete')   
  obsRed_BAMA2014S2_all =   rbindlist(  obsRed_BAMA2014S2,fill = TRUE, idcol = 'variete')   
  obsRed_BAMA2014S3_all =   rbindlist(  obsRed_BAMA2014S3 , fill = TRUE,idcol = 'variete')  
  obsRed_BAMA2015S1_all =  rbindlist(   obsRed_BAMA2015S1 ,fill = TRUE, idcol = 'variete') 
  
  obsRed_BBY2013S2_all =   rbindlist(   obsRed_BBY2013S2  , fill = TRUE,idcol = 'variete')
  
  obsRed_NIO2015S1_all =   rbindlist(   obsRed_NIO2015S1  ,fill = TRUE, idcol = 'variete')  
  obsRed_NIO2015S2_all =   rbindlist(   obsRed_NIO2015S2 , fill = TRUE,idcol = 'variete') 
  
  obsRed_SINT2013S1_all =  rbindlist(   obsRed_SINT2013S1 ,fill = TRUE, idcol = 'variete')
  obsRed_SINT2013S2_all =  rbindlist(   obsRed_SINT2013S2 ,fill = TRUE, idcol = 'variete')
  obsRed_SINT2014S1_all =  rbindlist(   obsRed_SINT2014S1 ,fill = TRUE, idcol = 'variete')
  obsRed_SINT2014S2_all =  rbindlist(   obsRed_SINT2014S2 ,fill = TRUE, idcol = 'variete')
  obsRed_SINT2015S1_all =  rbindlist(   obsRed_SINT2015S1 ,fill = TRUE, idcol = 'variete')  
  obsRed_SINT2015S2_all =  rbindlist(   obsRed_SINT2015S2 ,fill = TRUE, idcol = 'variete')  
  obsRed_SINT2016S1_all =  rbindlist(   obsRed_SINT2016S1 ,fill = TRUE, idcol = 'variete')
  
  obsRed_SMK2013S1_all =   rbindlist(   obsRed_SMK2013S1 ,fill = TRUE, idcol = 'variete')   
  obsRed_SMK2013S2_all =   rbindlist(   obsRed_SMK2013S2 ,fill = TRUE, idcol = 'variete')   
  obsRed_SMK2013S3_all =   rbindlist(   obsRed_SMK2013S3 ,fill = TRUE, idcol = 'variete')   
  obsRed_SMK2014S1_all =   rbindlist(   obsRed_SMK2014S1  ,fill = TRUE, idcol = 'variete')  
  obsRed_SMK2014S2_all =   rbindlist(   obsRed_SMK2014S2 ,fill = TRUE, idcol = 'variete')
  
  obsRed_BAMA2014S1_all$site = "BAMA2014S1"
  obsRed_BAMA2014S1_all$type = 'eval'
  obsRed_BAMA2014S2_all$site = "BAMA2014S2"
  obsRed_BAMA2014S2_all$type= 'eval'
  obsRed_BAMA2014S3_all$site = "BAMA2014S3"
  obsRed_BAMA2014S3_all$type= 'eval'
  obsRed_BAMA2015S1_all$site = "BAMA2015S1"
  obsRed_BAMA2015S1_all$type= 'eval'
  
  obsRed_BBY2013S2_all$site = "BBY2013S2"
  obsRed_BBY2013S2_all$type= 'eval'
  obsRed_NIO2015S1_all$site = "NIO2015S1"
  obsRed_NIO2015S1_all$type= 'eval'
  obsRed_NIO2015S2_all$site = "NIO2015S2"
  obsRed_NIO2015S2_all$type= 'valid'
  
  
  obsRed_SINT2013S1_all$site = "SINT2013S1"
  obsRed_SINT2013S1_all$type= 'valid'
  obsRed_SINT2013S2_all$site = "SINT2013S2"
  obsRed_SINT2013S2_all$type= 'valid'
  obsRed_SINT2014S1_all$site = "SINT2014S1"
  obsRed_SINT2014S1_all$type= 'calib'
  obsRed_SINT2014S2_all$site = "SINT2014S2"
  obsRed_SINT2014S2_all$type= 'calib'
  obsRed_SINT2015S1_all$site = "SINT2015S1"
  obsRed_SINT2015S1_all$type= 'eval'
  obsRed_SINT2015S2_all$site = "SINT2015S2"
  obsRed_SINT2015S2_all$type= 'valid'
  obsRed_SINT2016S1_all$site = "SINT2016S1"
  obsRed_SINT2016S1_all$type= 'calib'
  
  
  obsRed_SMK2013S1_all$site = "SMK2013S1"
  obsRed_SMK2013S1_all$type= 'eval'
  obsRed_SMK2013S2_all$site = "SMK2013S2"
  obsRed_SMK2013S2_all$type= 'eval'
  obsRed_SMK2013S3_all$site = "SMK2013S3"
  obsRed_SMK2013S3_all$type= 'eval'
  obsRed_SMK2014S1_all$site = "SMK2014S1"
  obsRed_SMK2014S1_all$type= 'eval'
  obsRed_SMK2014S2_all$site = "SMK2014S2"
  obsRed_SMK2014S2_all$type= 'eval'
  
  
  
  
  
  obs_Red_all = dplyr::bind_rows (obsRed_BAMA2014S1_all, 
                       obsRed_BAMA2014S2_all,
                       obsRed_BAMA2014S3_all,
                       obsRed_BAMA2015S1_all,
                       
                       obsRed_BBY2013S2_all, 
                       
                       obsRed_NIO2015S1_all, 
                       obsRed_NIO2015S2_all, 
                       
                       obsRed_SINT2013S1_all,
                       obsRed_SINT2013S2_all,
                       obsRed_SINT2014S1_all,
                       obsRed_SINT2014S2_all,
                       obsRed_SINT2015S1_all,
                       obsRed_SINT2015S2_all,
                       obsRed_SINT2016S1_all,
                       
                       obsRed_SMK2013S1_all, 
                       obsRed_SMK2013S2_all, 
                       obsRed_SMK2013S3_all, 
                       obsRed_SMK2014S1_all, 
                       obsRed_SMK2014S2_all )
  
  library("ggpubr")
  plot_list = list() #create an array to store all figures
  for (cat in unique(list_trial$trial))
  {
    sim <- subset(results_all, site == cat)
    obs <- subset(obs_Red_all, site == cat)
    plot_variable = ggplot(data=sim, aes (x =NbJas , y=Lai, 
                                          color = variete)) +
      geom_line()+ 
      geom_point(data=obs, aes( x =nbjas , y=lai))+
      facet_wrap(~variete, ncol = 2)+
      theme_bw()
    
    Final_plot_variable= annotate_figure(plot_variable,
                                         top = text_grob(paste0(cat,'_',sim$type), color = "red", 
                                                         face = "bold", size = 14))
    plot_list[[cat]] =  Final_plot_variable
  }
  
  pdf("Plot_lai_observed.pdf")
  for (cat in unique(list_trial$trial)) 
  {  print( plot_list[[cat]])}
  dev.off()
  
  
  
  ggplot(station_sim_all, aes(x=station,
                              y=CstrPhase5,
                              color = variete)) +
    geom_point()+
    geom_line()+
    coord_flip() +
    #facet_wrap(~variete, ncol=5)+
    theme_bw()
  
  ####exploration in one simulation
  ggplot(bama2014S1_Fadda, aes(x=NbJas, y=CstrPhase5)) +
    geom_line() +
    geom_point()+
    theme_bw()
  
  ggplot(bama2014S1_Fadda, aes(x=NumPhase)) +
    geom_line(aes(y=Cstr)) +
    geom_point(aes(y=CstrPhase2, color= "green"))+
    geom_point(aes(y=CstrPhase3, color= "red"))+
    geom_point(aes(y=CstrPhase4, color= "blue"))+
    geom_point(aes(y=CstrPhase5, color= "yellow"))+
    theme_bw()
  
  results_all_sum= ddply(results_all,c("site","variete"), 
                         summarise,  
                         mean_cstr = mean(Cstr),  
                         grain_yield   = max(GrainYieldPop),
                         biomass  = max(DryMatAboveGroundPop))
  
  ggplot(results_all_sum, aes(x=reorder(site,mean_cstr),
                              y=mean_cstr,
                          color = variete)) +
      geom_point()+
    geom_line()+
    coord_flip() +
     facet_wrap(~variete, ncol=5)+
    theme_bw()
  
  ggplot(results_all_sum, aes(x=reorder(site,mean_cstr),
                              y=grain_yield,
                              color = variete)) +
    geom_point()+
    geom_line()+
    coord_flip() +
    geom_point(aes(x=reorder(site,mean_cstr), y=biomass,
               color = variete), shape = 2)+
    facet_wrap(~variete, ncol=5)+
    theme_bw()
  
  ggplot(results_all_sum, aes(x=biomass,
                              y=grain_yield,
                              color = variete)) +
    geom_point()+
    facet_wrap(~site, ncol=5)+
    theme_bw()
  
  write.table(results_all, file = 'results_run_calib_malick_allsites.txt')
  write.table(results_all_sum, file = 'results_run_calib_malick_allsites_yield_cstr.txt')
  
#### 8juillet 2020
  ######setting up a sensitivity analysis on Asstr
  result_BAMA2014S1_01 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.1
      result_BAMA2014S1_01[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
      result_BAMA2014S1_01[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
      }
  
  result_BAMA2014S1_02 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.2
    result_BAMA2014S1_02[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_02[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  result_BAMA2014S1_03 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.3
    result_BAMA2014S1_03[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_03[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  
  result_BAMA2014S1_04 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.4
    result_BAMA2014S1_04[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_04[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  result_BAMA2014S1_05 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.5
    result_BAMA2014S1_05[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_05[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  result_BAMA2014S1_06 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.6
    result_BAMA2014S1_06[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_06[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  result_BAMA2014S1_07 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.7
    result_BAMA2014S1_07[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_07[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  result_BAMA2014S1_08 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.8
    result_BAMA2014S1_08[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_08[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
    }
  
  result_BAMA2014S1_09 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 0.9
    result_BAMA2014S1_09[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_09[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
  result_BAMA2014S1_10 = list()
  for (var in unique(list_variete$variete))
  {
    paramInit_BAMA2014S1[[var]]$asscstr = 1
    result_BAMA2014S1_10[[var]] <- rsamara::run2DF(paramInit_BAMA2014S1[[var]],meteoBama2014)
    result_BAMA2014S1_10[[var]]$AssCSTR = paramInit_BAMA2014S1[[var]]$asscstr
  }
  
sensibility_BAMA2014S1_01 = rbindlist(result_BAMA2014S1_01, idcol = 'variete')
sensibility_BAMA2014S1_02 = rbindlist(result_BAMA2014S1_02, idcol = 'variete')
sensibility_BAMA2014S1_03 = rbindlist(result_BAMA2014S1_03, idcol = 'variete')
sensibility_BAMA2014S1_04 = rbindlist(result_BAMA2014S1_04, idcol = 'variete')
sensibility_BAMA2014S1_05 = rbindlist(result_BAMA2014S1_05, idcol = 'variete')
sensibility_BAMA2014S1_06 = rbindlist(result_BAMA2014S1_06, idcol = 'variete')
sensibility_BAMA2014S1_07 = rbindlist(result_BAMA2014S1_07, idcol = 'variete')
sensibility_BAMA2014S1_08 = rbindlist(result_BAMA2014S1_08, idcol = 'variete')
sensibility_BAMA2014S1_09 = rbindlist(result_BAMA2014S1_09, idcol = 'variete')
sensibility_BAMA2014S1_10 = rbindlist(result_BAMA2014S1_10, idcol = 'variete')



sensibility_BAMA2014S1 = rbind (sensibility_BAMA2014S1_01,
                                sensibility_BAMA2014S1_02,
                                sensibility_BAMA2014S1_03,
                                sensibility_BAMA2014S1_04,
                                sensibility_BAMA2014S1_05,
                                sensibility_BAMA2014S1_06,
                                sensibility_BAMA2014S1_07,
                                sensibility_BAMA2014S1_08,
                                sensibility_BAMA2014S1_09,
                                sensibility_BAMA2014S1_10)


sensibility_BAMA2014S1_sum= ddply(sensibility_BAMA2014S1,c("AssCSTR","variete"), 
                       summarise,  
                       mean_cstr = mean(Cstr),  
                       grain_yield   = max(GrainYieldPop),
                       biomass  = max(DryMatAboveGroundPop))


ggplot(sensibility_BAMA2014S1, aes(x=NbJas, y=GrainYieldPop, 
                color = factor(AssCSTR), group =factor(AssCSTR))) +
  geom_line()+
  geom_point()+
  facet_wrap(~variete, nrow =2)+
  theme_bw()


ggplot(sensibility_BAMA2014S1, aes(x=FTSW, y=Cstr, 
                                   color = factor(AssCSTR), group =factor(AssCSTR))) +
  geom_line()+
  facet_wrap(~variete, nrow =2)+
  theme_bw()

ggplot(sensibility_BAMA2014S1, aes(x=FTSW, y=Cstr, 
                                   color = variete)) +
  geom_line()+
  #facet_wrap(~variete, nrow =2)+
  theme_bw()

sensibility_BAMA2014S1_Nieleni = subset(sensibility_BAMA2014S1,
                                      variete =='Nieleni')

ggplot(sensibility_BAMA2014S1_Nieleni, aes(x=NbJas, y=DryMatAboveGroundPop, 
                                   color = factor(AssCSTR), group =factor(AssCSTR))) +
  geom_point()+
  #facet_wrap(~AssCSTR, nrow =2)+
  theme_bw()


ggplot(sensibility_BAMA2014S1_sum, aes(x=mean_cstr,
                            y=biomass,
                            color = factor(AssCSTR))) +
  geom_point()+
  geom_line()+
  #facet_wrap(~variete, ncol=5)+
  theme_bw()


####  august: analysis on Cstr
ggplot(results_all, aes(x=NbJas, y=Cstr, color = site)) +
  geom_line() +
  geom_point()+
  facet_wrap(~variete, nrow =2)+
  theme_bw()


ggplot(results_all, aes(x=NbJas, y=Cstr, color = variete)) +
  geom_line() +
  geom_point()+
  facet_wrap(~site, nrow =2)+
  theme_bw()

ggplot(results_all, aes(x=NbJas, y=CstrPhase5, color = site)) +
  geom_line() +
  geom_point()+
  facet_wrap(~variete, nrow =2)+
  theme_bw()


ggplot(results_all, aes(x=site, y=CstrPhase4, color = variete)) +
  #geom_line() +
  geom_point()+
  coord_flip() +
  theme_bw()

results_all$sowing = as.factor (substr(results_all$site, 9, 11))
levels(results_all$sowing)[levels(results_all$sowing) %in%  c("1","S1")] <- "sowing1"
levels(results_all$sowing)[levels(results_all$sowing) %in%  c("2","S2")] <- "sowing2"
levels(results_all$sowing)[levels(results_all$sowing) %in%  c("3","S3")] <- "sowing3"

results_all$station = as.factor (substr(results_all$site, 1, 4))
levels(results_all$station)[levels(results_all$station) %in%  "BBY2"] <- "BBY"
levels(results_all$station)[levels(results_all$station) %in%  "FRK2"] <- "FRK"
levels(results_all$station)[levels(results_all$station) %in% "NIO2"] <- "NIO"
levels(results_all$station)[levels(results_all$station) %in%  "SMK2"] <- "SMK"

results_all$year = as.factor (substr(results_all$site, 5,8))
levels(results_all$year)[levels(results_all$year) %in%  "013S"] <- "2013"
levels(results_all$year)[levels(results_all$year) %in%  "014S"] <- "2014"
levels(results_all$year)[levels(results_all$year) %in%  "015S"] <- "2015"

results_all_sumCSTR= ddply(results_all,c("station","variete"), 
                           summarise,  
                           mean_cstr = mean(Cstr))

results_all_CSTRphase= ddply(results_all,c("station","year","sowing", "variete"), 
                             summarise,  
                             maxCstrPhase2 = max(CstrPhase2),
                             maxCstrPhase3 = max(CstrPhase3),
                             maxCstrPhase4 = max(CstrPhase4),
                             maxCstrPhase5 = max(CstrPhase5),
                             maxCstrPhase6 = max(CstrPhase6))

results_all_sumCSTRphase2= ddply(results_all_CSTRphase,c("station","variete"), 
                                 summarise,  N    = length(maxCstrPhase2),
                                 CstrPhase = mean( maxCstrPhase2),
                                 CstrPhasesd = sd( maxCstrPhase2), sePhase =CstrPhasesd/ sqrt(N),
                                 phase =2)
results_all_sumCSTRphase3= ddply(results_all_CSTRphase,c("station","variete"), 
                                 summarise,  N    = length(maxCstrPhase3),
                                 CstrPhase = mean( maxCstrPhase3),
                                 CstrPhasesd = sd( maxCstrPhase3), sePhase =CstrPhasesd/ sqrt(N),
                                 phase =3)
results_all_sumCSTRphase4= ddply(results_all_CSTRphase,c("station","variete"), 
                                 summarise,  N    = length(maxCstrPhase4),                           
                                 CstrPhase = mean( maxCstrPhase4),
                                 CstrPhasesd = sd( maxCstrPhase4), sePhase =CstrPhasesd/ sqrt(N),
                                 phase =4)
results_all_sumCSTRphase5= ddply(results_all_CSTRphase,c("station","variete"), 
                                 summarise,  N    = length(maxCstrPhase5),                          
                                 CstrPhase = mean( maxCstrPhase5),
                                 CstrPhasesd = sd( maxCstrPhase5), sePhase =CstrPhasesd/ sqrt(N),
                                 phase= 5)
results_all_sumCSTRphase6= ddply(results_all_CSTRphase,c("station","variete"), 
                                 summarise,  N    = length(maxCstrPhase6),                         
                                 CstrPhase = mean( maxCstrPhase6),
                                 CstrPhasesd = sd( maxCstrPhase6), sePhase =CstrPhasesd/ sqrt(N),
                                 phase =6)

results_all_sumCSTRphase = rbind(results_all_sumCSTRphase2,results_all_sumCSTRphase3,
                                 results_all_sumCSTRphase4,results_all_sumCSTRphase5,
                                 results_all_sumCSTRphase6)


output_50years_simul_sites<<-read.table("D:/Marksim/Analyse_august2020/output_sites50year_10varieties.txt",
                                        sep='\t',header=TRUE)

old_results_sites<<-read.table("D:/Marksim/Analyse_august2020/results_run_calib_malick_allsites.txt",
                                        sep=' ',header=TRUE)


output_50years_sumCSTRphase2= ddply( output_50years_simul_sites,c("station",".id"), 
                                     summarise,  N    = length(max.as.numeric.as.character.results_Samara_y.CstrPhase2...na.rm...TRUE..),
                                     CstrPhase = mean( max.as.numeric.as.character.results_Samara_y.CstrPhase2...na.rm...TRUE..),
                                     CstrPhasesd = sd( max.as.numeric.as.character.results_Samara_y.CstrPhase2...na.rm...TRUE..), sePhase =CstrPhasesd/ sqrt(N),
                                     phase =2)
output_50years_sumCSTRphase3= ddply( output_50years_simul_sites,c("station",".id"), 
                                     summarise,  N    = length(max.as.numeric.as.character.results_Samara_y.CstrPhase3...na.rm...TRUE..),
                                     CstrPhase = mean( max.as.numeric.as.character.results_Samara_y.CstrPhase3...na.rm...TRUE..),
                                     CstrPhasesd = sd( max.as.numeric.as.character.results_Samara_y.CstrPhase3...na.rm...TRUE..), sePhase =CstrPhasesd/ sqrt(N),
                                     phase =3)
output_50years_sumCSTRphase4= ddply( output_50years_simul_sites,c("station",".id"), 
                                     summarise,  N    = length(max.as.numeric.as.character.results_Samara_y.CstrPhase4...na.rm...TRUE..),                           
                                     CstrPhase = mean( max.as.numeric.as.character.results_Samara_y.CstrPhase4...na.rm...TRUE..),
                                     CstrPhasesd = sd( max.as.numeric.as.character.results_Samara_y.CstrPhase4...na.rm...TRUE..), sePhase =CstrPhasesd/ sqrt(N),
                                     phase =4)
output_50years_sumCSTRphase5= ddply( output_50years_simul_sites,c("station",".id"), 
                                     summarise,  N    = length(max.as.numeric.as.character.results_Samara_y.CstrPhase5...na.rm...TRUE..),                          
                                     CstrPhase = mean( max.as.numeric.as.character.results_Samara_y.CstrPhase5...na.rm...TRUE..),
                                     CstrPhasesd = sd( max.as.numeric.as.character.results_Samara_y.CstrPhase5...na.rm...TRUE..), sePhase =CstrPhasesd/ sqrt(N),
                                     phase= 5)
output_50years_sumCSTRphase6= ddply( output_50years_simul_sites,c("station",".id"), 
                                     summarise,  N    = length(max.as.numeric.as.character.results_Samara_y.CstrPhase6...na.rm...TRUE..),                         
                                     CstrPhase = mean( max.as.numeric.as.character.results_Samara_y.CstrPhase6...na.rm...TRUE..),
                                     CstrPhasesd = sd( max.as.numeric.as.character.results_Samara_y.CstrPhase6...na.rm...TRUE..), sePhase =CstrPhasesd/ sqrt(N),
                                     phase =6)

output_50years_sumCSTRphase = rbind(output_50years_sumCSTRphase2,output_50years_sumCSTRphase3,
                                    output_50years_sumCSTRphase4,output_50years_sumCSTRphase5,
                                    output_50years_sumCSTRphase6)

ggplot(output_50years_sumCSTRphase , aes(x=phase, y= CstrPhase )) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul 50 ans")+
  facet_grid(station~.id)+
  theme_bw()

ggplot(results_all_sumCSTRphase , aes(x=phase, y= CstrPhase )) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul calib")+
  facet_grid(station~variete)+
  theme_bw()

output_50years_sumCSTRphase$simul = '50years'
names(output_50years_sumCSTRphase)[names(output_50years_sumCSTRphase) == ".id"] <- "variete"
results_all_sumCSTRphase$simul= 'observed_sites'

comp_simul50years_obs = rbind(output_50years_sumCSTRphase,results_all_sumCSTRphase)

ggplot(comp_simul50years_obs , aes(x=phase, y= CstrPhase, color= simul )) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul calib")+
  facet_grid(station~variete)+
  theme_bw()


ggplot(comp_simul50years_obs , aes(x=phase, y= CstrPhase, color= simul)) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul calib")+
  facet_grid(station~variete)+
  theme_bw()

ggplot(output_50years_sumCSTRphase , aes(x=phase, y= CstrPhase, color= station)) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul 50 ans")+
  facet_wrap(~variete, nrow =2)+
  theme_bw()

ggplot(results_all_sumCSTRphase , aes(x=phase, y= CstrPhase, color= station)) +
  geom_line() +
  geom_point()+
  #scale_colour_manual( name="CLUSTER")+
  geom_errorbar(aes(ymin=CstrPhase-CstrPhasesd , ymax=CstrPhase+CstrPhasesd ), width=.2) +
  #xlim(1, 6)+  #  ggtitle("Interaction N*W") +
  # ylab("Grain yield (kg.ha-1") + xlab("fertilization treamtents")+
  ggtitle( "simul 50 ans")+
  facet_wrap(~variete, nrow =2)+
  theme_bw()


ggplot(comp_simul50years_obs, aes(x=variete,
                                  y=CstrPhase,
                                  color = station)) +
  geom_point()+
  geom_line()+
  coord_flip() +
  facet_grid(simul~phase)+
  theme_bw()
