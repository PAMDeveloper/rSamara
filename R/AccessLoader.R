

#accessFilePath = 'D:/BdD_Sorghum_10geno.accdb'

#Install and load packages
list.of.packages <- c("RODBC", "gdata")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)


#DB connection and load params & obs
initSim <- function(accessFilePath, simcode)
{
  connectDB(accessFilePath)
  .GlobalEnv$params <- loadSim(simcode)
  .GlobalEnv$meteo <- loadSimMeteo(simcode)
  .GlobalEnv$obs <- loadSimObs(simcode)
  odbcCloseAll()
}

initSimDetails <- function(accessFilePath, itkcode, variety, fieldcode, wscode, startDate, endDate)
{
  connectDB(accessFilePath)
  .GlobalEnv$params <- loadSimDetails(itkcode, variety, fieldcode, wscode, startDate, endDate)
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
julianDayLoad <- function(dateStr)
{
  dateStr = gsub("/","-",dateStr)
  res = toJulianDayCalc(dateStr)
  res
}

julianDayFromDB <- function(dateStr)
{
  res = toJulianDayCalc(format(dateStr, format="%Y-%m-%d"))
  res
}


loadSimParams <- function(sim, path)
{
  df = read.xls(path)
  df[which(df$simcode == sim), ]
 # sqlQuery(msAccessCon,paste("SELECT * FROM simulation WHERE simcode='", sim,"'", sep=""))
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

loadfield <- function(fieldcode)
{
  fieldDF = sqlQuery(msAccessCon,paste("SELECT * FROM plotsim WHERE fieldcode='", fieldcode,"'", sep=""))
  soilcode = fieldDF$soilcode[1]
  soil = sqlQuery(msAccessCon,paste("SELECT * FROM genesoil WHERE soilcode='", soilcode,"'", sep=""))
  merge(fieldDF, soil, by="soilcode")
}

#Native windows Fr access format on queries (MM-dd-YYYY) or (YYYY-MM-dd)
#Native windows Fr access format on result (YYYY-MM-dd)
loadMeteo <- function(wscode, beginDate, endDate)
{
  bDate = beginDate
  eDate = endDate
  query = paste("SELECT * FROM wdataday WHERE wscode='",wscode,"'",
                  " AND weatherdate > (#", bDate, "#)-2",
                  " AND weatherdate < #", eDate, "#",
                  " ORDER BY weatherdate ASC;",sep="")
  sqlQuery(msAccessCon,query)
}

loadObs <- function(trialcode, variety, startDate, endDate)
{

  query = paste("SELECT Observation_resultat.Id as id, Observation_resultat.obsplantdate as obsplantdate, ",
                "Observation_resultat.DAP as nbjas, [Observation_resultat]![Pl_Height]*10 AS plantheight, ",
                "Observation_resultat.lai as lai, Observation_resultat.GrainYield as grainyieldpop, ",
                "Observation_resultat.AppLeaves as appleaves, Observation_resultat.AppTill as culmsperplant,",
                " [Observation_resultat]![GrainYield]/[Observation_resultat]![TotBiom] AS haunindex, ",
                "Observation_resultat.PanGrainNb as drymatstructpaniclepop, ",
                "[Observation_resultat]![StemDM]*10+[Observation_resultat]![leafDM]*10+IIf(IsNull([Observation_resultat]![PanicleDM]*10),0,",
                "[Observation_resultat]![PanicleDM]*10) AS drymatabovegroundpop,",
                " [Observation_resultat]![lai]/([Observation_resultat]![leafDM]*10) AS sla,",
                " [Observation_resultat.leafDM]*10 as leafdm, [Observation_resultat.StemDM]*10 as stemdm, ",
                "[Observation_resultat.PanicleDM]*10 as panicledm FROM Observation_resultat "
                ," WHERE id='", trialcode,
                "' AND obsplantdate > (#", startDate, "#)-1",
                " AND obsplantdate < #", endDate, "#",
                sep="")
  # print(query)
  obs = sqlQuery(msAccessCon,query)

  #obs = sqlQuery(msAccessCon,paste("SELECT * FROM ObservationsSamara WHERE trialcode='", trialcode,
   #                         "' AND varcode='", variety, "'",
    #                        " AND obsplantdate > (#", startDate, "#)-2",
     #                       " AND obsplantdate < #", endDate, "#+1",
      #                      sep=""))
  obs$trialcode <- NULL
  obs$varcode <- NULL
  obs$obsplantdate <- sapply(obs$obsplantdate, julianDayFromDB)
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

loadSimDetails <- function(itkcode, variety, fieldcode, wscode, startDate, endDate)
{
  vardf = loadVariety(variety)
  itkdf = loadItk(itkcode)
  wsdf = loadws(wscode)
  fielddf = loadfield(fieldcode)

  # merge results
  #res = merge(sim, itkdf, by="itkcode")
  res = merge(itkdf, wsdf)
  res = merge(res, fielddf)
  res = merge(res, vardf)

  # clean df and set julian dates
  # print(res$sowing)
  res$sowing <- julianDayLoad(format(res$sowing, format="%Y/%m/%d"))
  res$endingdate <- julianDayLoad(endDate)
  res$startingdate <- julianDayLoad(startDate)
  res$itkcode <-NULL
  res$simcode <-NULL
  res$variety <- NULL
  res$soilcode <-NULL
  res$cropcode <-NULL
  res$fieldcode <-NULL
  res$name <-NULL
  res$namesoil <-NULL
  res$wscode <-NULL
  res$nblayer <-0
  res$p0 <-0
  # res$ru <-NULL
  res$depthsoil <-0

  res$kpar <- 0.5

  # res = res[, !apply(res==-999,2,all)]
  resDf <-as.data.frame(res)

  resDf
}

loadSim <- function(simCode, path)
{
  sim = loadSimParams(simCode, path);

  #get all df
  variety = sim$variety[1]
  itkcode = sim$itkcode[1]
  wscode = sim$wscode[1]
  fieldcode = sim$fieldcode[1]
  beginDate = sim$startingdate[1]
  endDate = sim$endingdate[1]
  #loadSimDetails(itkcode, variety, fieldcode, wscode, beginDate, endDate)

  vardf = loadVariety(variety)
  itkdf = loadItk(itkcode)
  wsdf = loadws(wscode)
  fielddf = loadfield(fieldcode)

  # merge results
  res = merge(sim, itkdf, by="itkcode")
  res = merge(res, wsdf, by="wscode")
  res = merge(res, fielddf, by="fieldcode")
  res = merge(res, vardf, by="variety", all.x=TRUE, all.y=FALSE)

  # clean df and set julian dates
  res$sowing <- julianDayLoad(format(res$sowing, format="%Y/%m/%d"))
  res$endingdate <- julianDayLoad(format(res$endingdate, format="%Y/%m/%d"))
  res$startingdate <- julianDayLoad(format(res$startingdate, format="%Y/%m/%d"))
  res$itkcode <-NULL
  res$simcode <-NULL
  res$variety <- NULL
  res$soilcode <-NULL
  res$cropcode <-NULL
  res$fieldcode <-NULL
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
