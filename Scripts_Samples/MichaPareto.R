#Script estimating parameters for Samara model
#Authors : Gregory Beurier -- (PAM, AGAP, BIOS, CIRAD)

#######################
######  DEFAULT  ######
#######################

list.of.packages <- c("rsamara", "RODBC", "parallel", "nsga2R", "nsga3")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')

######################
######  CONFIG  ######
######################

DB_PATH <- "D:/Bd_Samara.accdb"
PARAM_PATH <- "D:/Samples/SAMARA_SAMPLES/MichaPareto_R/Estim.csv"
RUNS_PATH <- "D:/Samples/SAMARA_SAMPLES/MichaPareto_R/Runs.csv"

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

str_parameters_count <- as.numeric(config_df[1,1])
str_parameters_df <- config_df[2:(str_parameters_count + 1), 4:ncol(config_df)]
row.names(str_parameters_df) <- config_df[2:(str_parameters_count + 1), 1]

parameters_df <- config_df[(str_parameters_count + 3):nrow(config_df), 4:ncol(config_df)]
row.names(parameters_df) <- config_df[(str_parameters_count + 3):nrow(config_df), 1]

### Construct runs dataframe
run_df <- read.csv(file = RUNS_PATH, header = FALSE, sep = ";", stringsAsFactors=FALSE)


### Prepare data
meteos <- list()
parameters <- list()
str_parameters <- list()
env_simu <- new.env()
env_simu$simu_init <- list()
for(run_idx in 1:ncol(meteoSites)) {
  startingDates[1,run_idx] <- formatDate(startingDates[1,run_idx])
  endingDates[1,run_idx] <- formatDate(endingDates[1,run_idx])
  meteos[[run_idx]] <- loadMeteo(as.character(meteoSites[1,run_idx]), as.character(startingDates[1,run_idx]), as.character(endingDates[1,run_idx]))
  parameters[[run_idx]] <- t(parameters_df[run_idx])
  str_parameters[[run_idx]] <- t(str_parameters_df[run_idx])
  env_simu$simu_init[[run_idx]] <- FALSE
}

odbcCloseAll()


#############################
#######   ESTIMATION  #######
#############################

get_minimize_function <- function(simus, vars, simu_init) {

  minimize_function <- function(p){
    result_array <- rep(0, length(vars))
    for(i in 1:length(simus)) {
      idx <- as.numeric(simus[i])

      if(env_simu$simu_init[[idx]] == FALSE) {
        print("init simu")
        env_simu$simu_init[[idx]] <- TRUE
        params = as.data.frame(parameters[[idx]], stringsAsFactors=FALSE)
        str_params = as.data.frame(str_parameters[[idx]], stringsAsFactors=FALSE)
        params$startingdate <- rsamara::toJulianDayCalcC(formatDate(str_params$startingdate), "YMD", '-')
        params$endingdate <- rsamara::toJulianDayCalcC(formatDate(str_params$endingdate), "YMD", '-')
        params$sowing <- rsamara::toJulianDayCalcC(formatDate(str_params$sowing), "YMD", '-')
        meteo = meteos[[idx]]
        rsamara::init_sim_idx(idx, params, meteo, str_params)
      }

      rsamara::update_sim_idx(idx, p, ParamOfInterest)
      results <- rsamara::run_sim_idx(idx)

      for(j in 1:length(vars)) {
        val <- ( 1 / results[nrow(results),vars[j]] )
        result_array[j] <- result_array[j] +val
      }
    }
    return ( result_array/length(simus) )
  }
  return (minimize_function);
}


######################
#######   RUN  #######
######################

simu_runs = run_df[run_df[,1] != "",1]

for(idx in 1:length(simu_runs)) {
  set.seed(1337)

  simus = strsplit(simu_runs[idx], ",")[[1]];
  vars = run_df[run_df[,2] != "",2]

  print(simus)
  print(vars)
  print("###############")

  minimize_function <- get_minimize_function(simus, vars, simu_init);
  environment(minimize_function)

  r2 <- nsga2(minimize_function,
              length(ParamOfInterest),
              length(vars),
              generations=200,
              popsize=40,
              lower.bounds=Bounds[,1],
              upper.bounds=Bounds[,2]
              )

  for( j in 1:nrow(r2[["value"]]) ) {
    r2[["value"]][j, 1:ncol(r2[["value"]])] <- 1 / r2[["value"]][j, 1:ncol(r2[["value"]])]
    for( k in 1:length(simus) ) {
      rsamara::update_sim_idx(k, r2[["par"]][j,1:ncol(r2[["par"]])], ParamOfInterest)
      rsamara::run_sim_idx(k)
      rsamara::save_sim_idx(k, paste("Pareto/sdjmatu1_rescapa/params", toString(idx), "-opt", toString(j), "-env", toString(k), sep=""))
    }
  }

  w <- matrix(c(r2[["par"]],r2[["value"]]), ncol=(ncol(r2[["par"]]) + ncol(r2[["value"]])))
  colnames(w) <- c(ParamOfInterest, vars)
  write.csv(file = paste(sep = "", "Pareto/sdjmatu1_rescapa/Pareto_results_run",toString(idx),".csv"), w)
  print(r2[["value"]])
  print(r2[["par"]])
  plot(r2)
}

################################
#######   VIZUALISATION  #######
################################

