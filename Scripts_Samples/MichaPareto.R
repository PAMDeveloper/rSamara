#Script estimating parameters for Samara model
#Authors : Gregory Beurier -- (PAM, AGAP, BIOS, CIRAD)

######################
######  CONFIG  ######
######################

DB_PATH <- "D:/Bd_Samara.accdb"
MAIN_DIR <- "D:/Samples/SAMARA_SAMPLES/MichaPareto_R"

RUNS_FILENAME <- "Runs.csv"
ESTIM_FILENAME <- "Estim_rescapa_sdjmatu1.csv"
SAVE_DIR <- "Rescapa_Sdjmatu1"

PARAM_PATH <- file.path(MAIN_DIR, ESTIM_FILENAME)
RUNS_PATH <- file.path(MAIN_DIR, RUNS_FILENAME)
SAVE_PATH <- file.path(MAIN_DIR, SAVE_DIR)
dir.create(SAVE_PATH, showWarnings = FALSE)

#######################
######  PACKAGES  ######
#######################

list.of.packages <- c("rsamara", "RODBC", "parallel", "nsga2R", "nsga3", "stringr")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')


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

get_minimize_function <- function(simus, vars) {

  minimize_function <- function(p){
    result_array <- rep(0, length(vars))
    for(i in 1:length(simus)) {
      idx <- as.numeric(simus[i])

      if(env_simu$simu_init[[idx]] == FALSE) {
        print(paste("Init simulation:",toString(idx)))
        env_simu$simu_init[[idx]] <- TRUE
        params = as.data.frame(parameters[[idx]], stringsAsFactors=FALSE)
        str_params = as.data.frame(str_parameters[[idx]], stringsAsFactors=FALSE)
        meteo = meteos[[idx]]
        rsamara::init_sim_idx(idx, params, meteo, str_params)

        print(paste("Done:",str_params$startingdate, str_params$endingdate, str_params$sowing, sep = " "))
      }

      rsamara::update_sim_idx(idx, p, ParamOfInterest)
      # rsamara::print_sim_idx(idx);
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

  print(paste("Launching run", simus))

  minimize_function <- get_minimize_function(simus, vars);

  r2 <- nsga2(minimize_function,
              length(ParamOfInterest),
              length(vars),
              generations=200,
              popsize=40,
              lower.bounds=Bounds[,1],
              upper.bounds=Bounds[,2]
              )

  # denormalize
  for( j in 1:nrow(r2[["value"]]) ) {
    r2[["value"]][j, 1:ncol(r2[["value"]])] <- 1 / r2[["value"]][j, 1:ncol(r2[["value"]])]
  }

  SAVE_PATH_RUN <- file.path(SAVE_PATH, paste("Run_", toString(idx), sep=""))
  dir.create(SAVE_PATH_RUN, showWarnings = FALSE)

  for( k in 1:length(simus) ) {
    SAVE_PATH_LOC <- file.path(SAVE_PATH_RUN, paste("Params_", toString(k), sep=""))
    dir.create(SAVE_PATH_LOC, showWarnings = FALSE)
    for( j in 1:nrow(r2[["value"]]) ) {
      rsamara::update_sim_idx(k, r2[["par"]][j,], ParamOfInterest)
      rsamara::save_sim_idx(k, file.path(SAVE_PATH_LOC, paste("Point_",toString(j), sep="")))
    }
  }


  w <- matrix(c(r2[["par"]],r2[["value"]]), ncol=(ncol(r2[["par"]]) + ncol(r2[["value"]])))
  colnames(w) <- c(ParamOfInterest, vars)
  simu_name <- paste("Run", toString(idx), "_results", sep="")
  write.csv(file = paste(file.path(SAVE_PATH, simu_name, sep=""),".csv", sep=""), w)

  png(filename=paste(file.path(SAVE_PATH, simu_name, sep=""),".png", sep=""))
  plot(r2)
  dev.off()
  # print(r2[["value"]])
  # print(r2[["par"]])
  print(paste("Run", simus, "done."))
}

################################
#######   VIZUALISATION  #######
################################

