library(RODBC)


accessFilePath = 'D:/BdD_Sorghum_10geno.accdb'

# Ouverture de la connexion vers la base de donn?es GARP_Formation"
connectionString = paste('Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=', accessFilePath, sep='')
connection <- odbcDriverConnect(connectionString)

loadSimParams <- function(sim)
{
  sqlQuery(connection,paste("SELECT * FROM simulation WHERE simcode='", sim,"'", sep=""))
}


loadVariety <- function(variety)
{
  sqlQuery(connection,paste("SELECT * FROM variety WHERE variety='", variety,"'", sep=""))
}

loadItk <- function(itkcode)
{
  sqlQuery(connection,paste("SELECT * FROM itksim WHERE itkcode='", itkcode,"'", sep=""))
}

loadws <- function(wscode)
{
  sqlQuery(connection,paste("SELECT wscode, wsalt, wslat, wslong FROM ws WHERE wscode='", wscode,"'", sep=""))
}

loadplot <- function(plotcode)
{
  plotDF = sqlQuery(connection,paste("SELECT * FROM plotsim WHERE plotcode='", plotcode,"'", sep=""))
  soilcode = plotDF$soilcode[1]
  soil = sqlQuery(connection,paste("SELECT * FROM genesoil WHERE soilcode='", soilcode,"'", sep=""))
  merge(plotDF, soil, by="soilcode")
}

#Native windows Fr access format on queries (MM-dd-YYYY) or (YYYY-MM-dd)
#Native windows Fr access format on result (YYYY-MM-dd)
loadmeteo <- function(wscode, beginDate, endDate)
{
  bDate = beginDate # format(as.Date(beginDate, "%d/%m/%Y"), format="%B %d %Y");
  eDate = endDate #format(as.Date(endDate, "%d/%m/%Y"), format="%B %d %Y");
  query = paste("SELECT * FROM wdataday WHERE wscode='",wscode,"'",
                  " AND weatherdate > (#", bDate, "#)-2",
                  " AND weatherdate < #", eDate, "#+1",
                  " ORDER BY weatherdate ASC;",sep="")
  sqlQuery(connection,query)
}

loadobs <- function(trialcode, variety)
{
  sqlQuery(connection,paste("SELECT * FROM ObservationsSamara WHERE trialcode='", trialcode,"' AND varcode='", variety, "'", sep=""))
}


# YYYY-MM-dd
tojulianday <- function(dateStr)
{
  res = julian(as.Date(dateStr), as.Date("0001-01-01")) + 1721427
  res
}

loadSim <- function(simCode)
{
  sim = loadSimParams(simCode);

  # get all df
  variety = sim$variety[1]
  vardf = loadVariety(variety)

  itkcode = sim$itkcode[1]
  itkdf = loadItk(itkcode)

  wscode = sim$wscode[1]
  wsdf = loadws(wscode)

  plotcode = sim$plotcode[1]
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

  resDf <-as.data.frame(res)

  # transpose parameters dataframe
  #n <- res$name
  #res <- setNames(as.data.frame(t(res[,-1])), colnames(res))
  #colnames(res) <- c("val")
  resDf
}

loadSimMeteo <- function(simCode)
{
  sim = loadSimParams(simCode);

  wscode = sim$wscode[1]
  beginDate = sim$startingdate[1]
  endDate = sim$endingdate[1]

  loadmeteo(wscode,beginDate,endDate)
}

# odbcCloseAll()
