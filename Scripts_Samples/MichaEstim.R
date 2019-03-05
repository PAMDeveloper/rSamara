#Script estimating parameters for Samara model
#Authors : Gregory Beurier -- (PAM, AGAP, BIOS, CIRAD)

#######################
######  DEFAULT  ######
#######################

list.of.packages <- c("rsamara", "RODBC", "parallel", "DEoptim")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')

######################
######  CONFIG  ######
######################

DB_PATH <- "D:/Bd_Samara.accdb"
PARAM_PATH <- "D:/Samples/SAMARA_SAMPLES/MichaEstim_R/Estim.csv"
OBS_PATH <- "D:/Samples/SAMARA_SAMPLES/MichaEstim_R/Obs.csv"

############################
#######   LOAD DATA  #######
############################
connectDB(DB_PATH)

### Construct parameters dataframe
config_df <- read.csv(file = PARAM_PATH, header = FALSE, sep = ";", stringsAsFactors=FALSE)
POI_array <- config_df[!is.na(config_df[,2]),]
ParamOfInterest <- as.character(POI_array[,1])
NbParam <- length(ParamOfInterest)
MinValue <- as.numeric(as.character(POI_array[,2]))
MaxValue <- as.numeric(as.character(POI_array[,3]))
Bounds <- matrix(c(MinValue,MaxValue),ncol=2)

meteoSites <- subset(config_df, config_df[,1]=="wscode")
meteoSites <- meteoSites[4:ncol(meteoSites)]
startingDates <- subset(config_df, config_df[,1]=="startingdate")
startingDates <- startingDates[1,4:ncol(startingDates)]
endingDates <- subset(config_df, config_df[,1]=="endingdate")
endingDates <- endingDates[1,4:ncol(endingDates)]
sowingDates <- subset(config_df, config_df[,1]=="sowing")
sowingDates <- sowingDates[1,4:ncol(sowingDates)]

str_parameters_count <- as.numeric(config_df[1,1])
str_parameters_df <- config_df[2:(str_parameters_count + 1), 4:ncol(config_df)]
row.names(str_parameters_df) <- config_df[2:(str_parameters_count + 1), 1]

parameters_df <- config_df[(str_parameters_count + 3):nrow(config_df), 4:ncol(config_df)]
row.names(parameters_df) <- config_df[(str_parameters_count + 3):nrow(config_df), 1]

### Construct observations dataframe
obs_df <- read.csv(file = OBS_PATH, header = TRUE, sep = ";", stringsAsFactors=FALSE)
obs_df <- obs_df[(obs_df[,1] != ""),]

getObs <- function(run_idx){
  obs_subset_df <- obs_df[,c(1, run_idx + 1)]
  obs_rows <- subset(obs_subset_df, grepl("Time*", obs_subset_df[,1]))
  obs_rows <- unique(as.character(obs_rows[,2]))
  obs_cols <- subset(obs_subset_df, !grepl("Time*", obs_subset_df[,1]))
  obs_cols <- unique(as.character(obs_cols[,1]))

  final_obs <- matrix(nrow = length(obs_rows), ncol=(length(obs_cols) + 1))
  final_df <- as.data.frame(final_obs)
  colnames(final_df) <- c("nbjas", obs_cols)
  row.names(final_df) <- obs_rows

  obs_rows[obs_rows == "end"] <- -1
  final_df$nbjas <- as.numeric(obs_rows)


  colName <- ""
  value <- NA
  for (row in 1:nrow(obs_subset_df)) {
    if (row %% 2 == 0) {
      rowName <- as.character(obs_subset_df[row, 2])
      final_df[rowName, colName] <- value
    } else {
      colName <- as.character(obs_subset_df[row, 1])
      value <- as.numeric(sub(",", ".", obs_subset_df[row, 2], fixed = TRUE))
    }
  }

  final_df <- final_df[ , order(names(final_df))]
  return (final_df)
}

### Prepare data
meteos <- list()
observations <- list()
parameters <- list()
str_parameters <- list()
env_simu <- new.env()
env_simu$simu_init <- list()
for(run_idx in 1:ncol(meteoSites)) {
  observations[[run_idx]] <- getObs(run_idx)

  startingDates[1,run_idx] <- formatDate(startingDates[1,run_idx])
  endingDates[1,run_idx] <- formatDate(endingDates[1,run_idx])
  sowingDates[1,run_idx] <- formatDate(sowingDates[1,run_idx])

  meteos[[run_idx]] <- loadMeteo(as.character(meteoSites[1,run_idx]), as.character(startingDates[1,run_idx]), as.character(endingDates[1,run_idx]))

  parameters_df["startingdate", run_idx] <- rsamara::toJulianDayCalcC(startingDates[1,run_idx], "YMD", '-')
  parameters_df["endingdate", run_idx] <- rsamara::toJulianDayCalcC(endingDates[1,run_idx], "YMD", '-')
  parameters_df["sowing", run_idx] <- rsamara::toJulianDayCalcC(sowingDates[1,run_idx], "YMD", '-')
  parameters[[run_idx]] <- t(parameters_df[run_idx])

  str_parameters_df["startingdate", run_idx] <- startingDates[1,run_idx]
  str_parameters_df["endingdate", run_idx] <- endingDates[1,run_idx]
  str_parameters_df["sowing", run_idx] <- sowingDates[1,run_idx]
  str_parameters[[run_idx]] <- t(str_parameters_df[run_idx])

  env_simu$simu_init[[run_idx]] <- FALSE
}

odbcCloseAll()


#############################
#######   ESTIMATION  #######
#############################

get_fitness_function <- function(idx) {

  fitness_function <- function(p){

    if(env_simu$simu_init[[idx]] == FALSE) {
      print("init simu")
      obs = observations[[idx]]
      params = as.data.frame(parameters[[idx]], stringsAsFactors=FALSE)
      str_params = as.data.frame(str_parameters[[idx]], stringsAsFactors=FALSE)
      meteo = meteos[[idx]]
      env_simu$simu_init[[idx]] <- TRUE
      rsamara::init_sim_idx(idx, params, meteo, str_params)
      print(paste(str_params$startingdate, str_params$endingdate, str_params$sowing, sep = " "))
    }

    rsamara::update_sim_idx(idx, p, ParamOfInterest)
    results <- rsamara::run_sim_idx(idx)

    lai <- results$Lai[results$NbJAS == 65]
    if(lai < 3.5)
      return (99999)

    res <- rsamara::reduce_sim(results, obs)

    robs <- obs
    robs$nbjas <- NULL
    rres <- res
    rres$nbjas <- NULL

    # rmae_diff <- ((obs[-nrow(obs),] - res[-nrow(obs),])/obs[-nrow(obs),])^2
    rmse_diff <- ( (robs - rres) / robs )^2
    # rmae = sum(sqrt((colSums(rmae_diff, na.rm=T))/(colSums(!is.na(rmae_diff)))),na.rm=T)
    rmse = (sum(sqrt((colSums(rmse_diff, na.rm=T))/(colSums(!is.na(rmse_diff)))),na.rm=T))
    # return (rmae + rmse)
    return (rmse)
  }

  return (fitness_function);
}


######################
#######   RUN  #######
######################
cl <- makeCluster(detectCores())
clusterEvalQ(cl, library(rsamara))

for(idx in 1:length(parameters)) {
  set.seed(1337)
  fitness_function <- get_fitness_function(idx);

  clusterExport(cl
                , varlist = c("ParamOfInterest", "env_simu", "idx", "observations", "parameters", "meteos", "str_parameters")
                )

  clusterExport(cl
                , varlist = c("simu_init")
                , envir = env_simu
  )

  DEParams = DEoptim.control( VTR = 0.001
                              , itermax = 5
                              , strategy = 2
                              , trace = 2
                              , NP = 100
                              , cluster=cl
                              )


  result <- DEoptim(fitness_function, lower=Bounds[,1], upper=Bounds[,2], control = DEParams)
  print(ParamOfInterest)
  print(result$optim$bestmem)
  print(result$optim$bestval)

  # rsamara::init_sim(params, meteo, str_params)
  rsamara::update_sim_idx(idx, result$optim$bestmem, ParamOfInterest)
  rsamara::save_sim(paste("Parameters_",toString(idx)))
#
  # rsamara::update_sim(result$optim$bestmem, ParamOfInterest)
  # fresults <- rsamara::run_sim()
  # fres <- rsamara::reduce_sim(fresults, obs)
  # frobs <- obs
  # frobs$nbjas <- NULL
  # fres$nbjas <- NULL

}

################################
#######   VIZUALISATION  #######
################################
