

#accessFilePath = 'D:/BdD_Sorghum_10geno.accdb'

#Install and load packages
list.of.packages <- c("RODBC")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)


initSim <- function(accessFilePath, simcode)
{
  connectDB(accessFilePath)
  .GlobalEnv$params <- loadSim(simcode)
  .GlobalEnv$meteo <- loadSimMeteo(simcode)
  .GlobalEnv$obs <- loadSimObs(simcode)
  odbcCloseAll()
}

initSimDetails <- function(accessFilePath, itkcode, variety, plotcode, wscode, startDate, endDate)
{
  connectDB(accessFilePath)
  .GlobalEnv$params <- loadSimDetails(itkcode, variety, plotcode, wscode, startDate, endDate)
  .GlobalEnv$meteo <- loadMeteo(wscode, startDate, endDate)
  .GlobalEnv$obs <- loadObs(itkcode, variety, startDate, endDate)
  odbcCloseAll()
}

connectDB <- function(accessFilePath)
{
  # Ouverture de la connexion vers la base de donn?es GARP_Formation"
  msAccessConString = paste('Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=', accessFilePath, sep='')
  .GlobalEnv$msAccessCon <- odbcDriverConnect(msAccessConString)
}

# YYYY-MM-dd
tojulianday <- function(dateStr)
{
  res = julian(as.Date(dateStr), as.Date("0001-01-01")) + 1721427
  res
}

fromjulian <-  function(jday)
{
  res = as.Date(jday - 1721427, origin=as.Date("0001-01-01"))
  res
}

loadSimParams <- function(sim)
{
  sqlQuery(msAccessCon,paste("SELECT * FROM simulation WHERE simcode='", sim,"'", sep=""))
}


loadVariety <- function(variety)
{
  sqlQuery(msAccessCon,paste("SELECT * FROM variety WHERE variety='", variety,"'", sep=""))
}

loadItk <- function(itkcode)
{
  sqlQuery(msAccessCon,paste("SELECT * FROM itksim WHERE itkcode='", itkcode,"'", sep=""))
}

loadws <- function(wscode)
{
  sqlQuery(msAccessCon,paste("SELECT wscode, wsalt, wslat, wslong FROM ws WHERE wscode='", wscode,"'", sep=""))
}

loadplot <- function(plotcode)
{
  plotDF = sqlQuery(msAccessCon,paste("SELECT * FROM plotsim WHERE plotcode='", plotcode,"'", sep=""))
  soilcode = plotDF$soilcode[1]
  soil = sqlQuery(msAccessCon,paste("SELECT * FROM genesoil WHERE soilcode='", soilcode,"'", sep=""))
  merge(plotDF, soil, by="soilcode")
}

#Native windows Fr access format on queries (MM-dd-YYYY) or (YYYY-MM-dd)
#Native windows Fr access format on result (YYYY-MM-dd)
loadMeteo <- function(wscode, beginDate, endDate)
{
  bDate = beginDate # format(as.Date(beginDate, "%d/%m/%Y"), format="%B %d %Y");
  eDate = endDate #format(as.Date(endDate, "%d/%m/%Y"), format="%B %d %Y");
  query = paste("SELECT * FROM wdataday WHERE wscode='",wscode,"'",
                  " AND weatherdate > (#", bDate, "#)-2",
                  " AND weatherdate < #", eDate, "#+1",
                  " ORDER BY weatherdate ASC;",sep="")
  sqlQuery(msAccessCon,query)
}

loadObs <- function(trialcode, variety, startDate, endDate)
{
  obs = sqlQuery(msAccessCon,paste("SELECT * FROM ObservationsSamara WHERE trialcode='", trialcode,
                            "' AND varcode='", variety, "'",
                            " AND obsplantdate > (#", startDate, "#)-2",
                            " AND obsplantdate < #", endDate, "#+1",
                            sep=""))
  obs$trialcode <- NULL
  obs$varcode <- NULL
  obs$obsplantdate <- sapply(obs$obsplantdate, tojulianday)
  obs <- obs[,colSums(is.na(obs))<nrow(obs)]
  obs
}

loadSimObs <- function(simcode)
{
  sim = loadSimParams(simcode);
  trialcode = sim$itkcode[1]
  variety = sim$variety[1]
  bDate = sim$startingdate[1]
  eDate = sim$endingdate[1]
  res = loadObs(trialcode, variety, bDate, eDate)
  res
}

loadSimDetails <- function(itkcode, variety, plotcode, wscode, startDate, endDate)
{
  vardf = loadVariety(variety)
  itkdf = loadItk(itkcode)
  wsdf = loadws(wscode)
  plotdf = loadplot(plotcode)

  # merge results
  #res = merge(sim, itkdf, by="itkcode")
  res = merge(itkdf, wsdf)
  res = merge(res, plotdf)
  res = merge(res, vardf)

  # clean df and set julian dates
  res$sowing <- tojulianday(res$sowing)
  res$endingdate <- tojulianday(endDate)
  res$startingdate <- tojulianday(startDate)
  res$itkcode <-NULL
  res$simcode <-NULL
  res$variety <- NULL
  res$soilcode <-NULL
  res$cropcode <-NULL
  res$plotcode <-NULL
  res$name <-NULL
  res$namesoil <-NULL
  res$wscode <-NULL
  res$nblayer <-NULL
  res$p0 <-NULL
  res$ru <-NULL
  res$depthsoil <-NULL

  res$kpar <- 0.5

  res = res[, !apply(res==-999,2,all)]
  resDf <-as.data.frame(res)

  resDf
}

loadSim <- function(simCode)
{
  sim = loadSimParams(simCode);

  #get all df
  variety = sim$variety[1]
  itkcode = sim$itkcode[1]
  wscode = sim$wscode[1]
  plotcode = sim$plotcode[1]
  beginDate = sim$startingdate[1]
  endDate = sim$endingdate[1]
  #loadSimDetails(itkcode, variety, plotcode, wscode, beginDate, endDate)

  vardf = loadVariety(variety)
  itkdf = loadItk(itkcode)
  wsdf = loadws(wscode)
  plotdf = loadplot(plotcode)

  # merge results
  res = merge(sim, itkdf, by="itkcode")
  res = merge(res, wsdf, by="wscode")
  res = merge(res, plotdf, by="plotcode")
  res = merge(res, vardf, by="variety", all.x=TRUE, all.y=FALSE)

  # clean df and set julian dates
  res$sowing <- tojulianday(res$sowing)
  res$endingdate <- tojulianday(res$endingdate)
  res$startingdate <- tojulianday(res$startingdate)
  res$itkcode <-NULL
  res$simcode <-NULL
  res$variety <- NULL
  res$soilcode <-NULL
  res$cropcode <-NULL
  res$plotcode <-NULL
  res$name <-NULL
  res$namesoil <-NULL
  res$wscode <-NULL
  res$nblayer <-NULL
  res$p0 <-NULL
  res$ru <-NULL
  res$depthsoil <-NULL

  res$kpar <- 0.5
  res
  #res = res[, !apply(res==-999,2,all)]
  #resDf <-as.data.frame(res)
  #resDf
}

loadSimMeteo <- function(simCode)
{
  sim = loadSimParams(simCode);

  wscode = sim$wscode[1]
  beginDate = sim$startingdate[1]
  endDate = sim$endingdate[1]

  loadMeteo(wscode,beginDate,endDate)
}

# odbcCloseAll()
