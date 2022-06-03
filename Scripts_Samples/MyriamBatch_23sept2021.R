########################
######  PACKAGES  ######
########################
list.of.packages <- c("rsamara","RODBC", "gdata")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)


############################
#######   LOAD DATA  #######
############################
#connectDB("D:/Workspace/PAM/rSamara/Myriam/Basedonnee_Malick_Myriam_july2020.accdb")
#sims = read.xls("D:/Workspace/PAM/rSamara/Myriam/simulation_23sites_AO_myriam.xlsx", check.names=FALSE)

connectDB("D:/Marksim/DATA_validation/Basedonnee_Malick_Myriam_july2020.accdb")
sims = read.xls("D:/Marksim/DATA_validation/simulation_23sites_AO_myriam.xlsx", check.names=FALSE)


launch <- function (row, variety, genotype) {
  row = trim(row)
  print(paste(row[2], row[3], row[4], row[5], row[6], variety, genotype))
  meteo <- loadMeteo(row[4], row[5], row[6])
  params = loadSimDetails(row[2], variety, row[3], row[4], row[5], row[6])
  params <- lapply(params, as.numeric)
  obs <- loadObs(paste(row[2],genotype,sep=""), variety, row[5], row[6])
  obs <- lapply(obs[,-1], as.numeric)

  results = rsamara::run2DF(params,meteo)
 
  obsRed <- rsamara::rcpp_reduceVobs(obs, results)
  resRed <- rsamara::rcpp_reduceResults(results, obs)
  
  results$site = row[2]
  results$variety= variety
  
  mypath = "D:/Marksim/DATA_validation/result_samara"
  setwd(mypath)
  result_filename = paste(variety, "_", row[2], ".txt")
  write.table(format(results, digits=18), result_filename, sep='\t',row.names=F)
  
  mypath = "D:/Marksim/DATA_validation/obs_red"
  setwd(mypath)
  obsRed_filename = paste(variety, "_", row[2], ".txt")
  write.table(format(obsRed, digits=18), obsRed_filename, sep='\t',row.names=F)
  
  mypath = "D:/Marksim/DATA_validation/res_red"
  setwd(mypath)
  resRed_filename = paste(variety, "_", row[2], ".txt")
  write.table(format(  resRed, digits=18), resRed_filename, sep='\t',row.names=F)
  
  # print(paste(filename, " written"))

}

for(i in 1:nrow(sims)) {
  var = sims$variety[i]
  geno = sims$genotype[i]
  if(var != "") {
    apply(sims, 1, launch, variety=var, genotype=geno)
  }
}
odbcCloseAll()


#######Myriam *** Merge txt files 
####resultats SAMARA daily output
mypath = "D:/Marksim/DATA_validation/result_samara"
setwd(mypath)
# Create list of text files
txt_files_ls = list.files(path=mypath, pattern="*.txt") 
# Read the files in, assuming tab separator
txt_files_df <- lapply(txt_files_ls, function(x) {read.table(file = x, header = T, sep ="\t")})
# Combine them
combined_result_samara <- do.call("rbind", lapply(txt_files_df, as.data.frame)) 
write.table(format(combined_result_samara, digits=18), "daily_result_samara_all_sites.txt", sep='\t',row.names=F)


###############################################################################################################
###############################################################################################################
#############################another solution for merging #####################################
###############################################################################################################
###############################################################################################################
# launch <- function (row, variety, genotype) {
#   row = trim(row)
#   print(paste(row[2], row[3], row[4], row[5], row[6], variety, genotype))
#   meteo <- loadMeteo(row[4], row[5], row[6])
#   params = loadSimDetails(row[2], variety, row[3], row[4], row[5], row[6])
#   params <- lapply(params, as.numeric)
#   obs <- loadObs(paste(row[2],genotype,sep=""), variety, row[5], row[6])
#   obs <- lapply(obs[,-1], as.numeric)
#   
#   results = rsamara::run2DF(params,meteo)
#   
#   obsRed <- rsamara::rcpp_reduceVobs(obs, results)
#   resRed <- rsamara::rcpp_reduceResults(results, obs)
#   
#   results$site = row[2]
#   results$variete = variety
#   
#   #filename = paste(variety, "_", row[2], ".csv")
#   #write.table(format(results, digits=18), filename, sep='\t',row.names=F)
#   # print(paste(filename, " written"))
#   return(results)
# }
# 
# 
# result = list()
# result_site= list()
# for(i in 1:nrow(sims)) {
#   var = sims$variety[i]
#   geno = sims$genotype[i]
#   if(var != "") {
#     result[[i]]= apply(sims, 1, launch, variety=var, genotype=geno)
#     result_site[[i]] = rbindlist( result[[i]], idcol= FALSE)
#   }
# }
# 
# final = rbindlist(result_site, idcol= FALSE)
# write.table(format(final, digits=18), "result_samara_all.txt", sep='\t',row.names=F)

