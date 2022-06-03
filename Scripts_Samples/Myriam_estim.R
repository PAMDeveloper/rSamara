########################
######  PACKAGES  ######
########################
list.of.packages <- c("rsamara","RODBC", "gdata", "parallel", "DEoptim", "stringr")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')

#######################
#######   DATA  #######
#######################

## path
local_path = "D:/Workspace/PAM/rSamara/Myriam/"
db_file = "Basedonnee_Malick_Myriam_july2020.accdb"
xls_file = "simulation_23sites_AO_myriam.xlsx"
result_folder = "result_samara"
dir.create(paste(local_path, result_folder), showWarnings = FALSE)

## load
connectDB(paste(local_path, db_file))
sims = read.xls(paste(local_path, xls_file), check.names=FALSE)

## prepare data
meteos <- list()
observations <- list()
parameters <- list()
str_parameters <- list()
env_simu <- new.env()
env_simu$simu_init <- list()

