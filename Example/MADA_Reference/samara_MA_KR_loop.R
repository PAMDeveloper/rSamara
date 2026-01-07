##############################################################################
##############################################################################
###############Scenario analysis for rice in Mada###########################
##########################Use of SAMARA #############################
##############################################################################
##############################################################################
##############Script by Myriam ADAM and Koloina  Rahajaharilaza ###########
###########################May 2025#####################################
##############################################################################
##############################################################################

library(samara)
library(ggplot2)
library(stats)
library(base)
library(dplyr)
library(lubridate) ## for date format

####read weather#####################
weather_Hadil <- read.csv("Meteo.csv", sep=";")
weather_Hadil$weatherdate <- as.Date(weather_Hadil$weatherdate,
                                     format = "%d/%m/%Y")

### separation param
####soil data###################################
param_soils <- read.csv("soil_Hadil.csv", sep=",")
#soils <- "DSrain3mm"
# we have 3 different soils representing different drainage abilities
soils<- c("DSrain3mm",  "DSrain6mm",  "DSrain12mm")

####itk data##############################
param_mngts <- read.csv("itk_Hadil_sowing.csv", sep=",")
#mngts <- "Transplanting"
# we have 4 management/irrigation practices
mngts <- c("Transplanting",  "DSirrg",  "Rainfed", "Rainfed_true")

##param nursery as Hadil
param_mngts$durationnursery =46
param_mngts$densitynursery = 50000000
#param_mngts$ftswirrig = 2

####sowing and simulation start/end
#sowing_dates <- "01/11"
# we test 11 sowing dates, representing a sowing every 2 weeks
#during the potential sowing window in Mad
# sowing_dates <- c("01/09", "15/09", "01/10", "15/10",
#                    "01/11", "15/11", "01/12", "15/12",
#                    "01/01","15/01","01/02")
sowing_dates <- c("01/10")

# to fix the period of simulation ( for the cropping season)
days_before_sowing <- 20
months_after_sowing <- 10

###varieties###############################
param_varieties <- read.csv("crop_Hadil.csv", sep=",")
param_varieties$hauncritstemelongation = 20 # a verifier/ implication
#varieties = "C1"
# according to Michael and Hadil work there is 3 types of varieties
# varieties <- c("C1",  "C2",  "C3")
varieties = "C1"



####set up for the loop
# years <- 2002:2024
years = 2002

all_results_MA_KR <- list()

for(sowing_date in sowing_dates ){
  for (year in years) {
    for(variety in varieties)
    {

          param_variety <- param_varieties%>% filter(ID_crop== variety)


          for (soil in soils)
          {

            param_soil<- param_soils%>% filter(ID_soil== soil)

            for (mngt in mngts){

              param_mngt<- param_mngts%>% filter(ID_ITK== mngt)


###merge param to run samara
# merge les 3 fichiers de parametres pour pouvoir run Samara
param_merge <- merge(param_soil,param_mngt,
                     by='row.names', all=T)
param_merge= param_merge[,-1] # pour enlever le row.names column
param_MA_KR<- merge (param_merge, param_variety,
                by='row.names', all=T)
param_MA_KR= param_MA_KR[,-1] # pour enlever le row.names column

param_MA_KR =param_MA_KR [,c(-1, -21, -44) ] #to remove the ID
param_MA_KR  <- param_MA_KR [, sort(names(param_MA_KR ))]##alphabetic order


###set up the sowing date and start.end simulation per cropping season
sowing_date_full <- as.Date(paste0(sowing_date, "/", year)
                            , format = "%d/%m/%Y")
start_date <- sowing_date_full - days_before_sowing
end_date <- sowing_date_full + months_after_sowing * 30  # approximating months with 30 days

param_MA_KR$sowing=sowing_date_full
param_MA_KR$startingdate=start_date
param_MA_KR$endingdate= end_date

# Calculate the Modified Julian Date (MJD)
param_MA_KR$sowing <- as.numeric(param_MA_KR$sowing) +2440587.5
param_MA_KR$startingdate <- as.numeric(param_MA_KR$startingdate) +2440587.5
param_MA_KR$endingdate <- as.numeric(param_MA_KR$endingdate) +2440587.5


# param_MA_KR$sowing <- as.Date(param_MA_KR$sowing, format = "%d/%m/%Y")
# param$startingdate <- as.Date(param$startingdate, format = "%d/%m/%Y")
# param$endingdate <- as.Date(param$endingdate, format = "%d/%m/%Y")

# select the weather data corresponding to the cropping season
filtered_weather <- weather_Hadil %>% filter(weatherdate >= start_date
                                             & weatherdate <= end_date)

# Initialiser les simulations
samara::init_sim_idx_simple(1, param_MA_KR, filtered_weather)

# Lancer les simulations 1 = init, 4 = modele version
res_MA_KR <- samara::run_sim_idx_version(1, 4)


res_MA_KR $Year=year
res_MA_KR $Variety = variety
res_MA_KR $Soil= soil
res_MA_KR $Mngt= mngt
res_MA_KR $SD=sowing_date

all_results_MA_KR <- append(all_results_MA_KR, list(res_MA_KR))
    } }
    }}}


combined_df_MA_KR <- do.call(rbind, all_results_MA_KR)
# write.csv(combined_df_MA_KR, file = "output_MA-KR.csv")

# check quickly the output of simulations
ggplot(combined_df_MA_KR , aes(x = NbJAS, y = GrainYieldPop,
                               color= as.factor(Variety))) +
  geom_line( )+
  facet_wrap(~combined_df_MA_KR$Year)

###summarize data for boxplot

df_max_MA_KR <- combined_df_MA_KR %>%
  group_by(Year,Variety,SD,Soil,Mngt) %>%
  summarize(max_value_GY = max(GrainYieldPop),
            max_value_lai = max(Lai),)
write.csv(df_max_MA_KR, file = "output_sumup_MA-KR.csv")

# Define the order of the categories
category_order <- c("01/09", "15/09", "01/10", "15/10",
                    "01/11", "15/11", "01/12", "15/12",
                    "01/01","15/01","01/02")
category_order_soil <- c("DSrain3mm",  "DSrain6mm",  "DSrain12mm")
category_order_mngt <- c("Transplanting",  "DSirrg",  "Rainfed", "Rainfed_true")

# Convert Category to a factor with the specified order
df_max_MA_KR$SD<- factor(df_max_MA_KR$SD, levels = category_order)
df_max_MA_KR$Soil<- factor(df_max_MA_KR$Soil, levels = category_order_soil)
df_max_MA_KR$Mngt<- factor(df_max_MA_KR$Mngt, levels = category_order_mngt)

ggplot(df_max_MA_KR, aes(x = SD, y = max_value_GY, color= Variety)) +
  geom_boxplot() +
  labs(title = "Box Plot of Maximum grain yield",
       x = "Sowing dates",
       y = "grain yield kg.ha-1") +
  facet_grid(df_max_MA_KR$Mngt~df_max_MA_KR$Soil)+
  theme_minimal()

ggplot(df_max_MA_KR, aes(x = SD, y = max_value_GY, color= Mngt)) +
  geom_boxplot() +
  labs(title = "Box Plot of Maximum grain yield",
       x = "Sowing dates",
       y = "grain yield kg.ha-1") +
  facet_grid(df_max_MA_KR$Variety~df_max_MA_KR$Soil)+
  theme_minimal()


####select for one year and one SD
combined_df_MA_KR_2020 <- combined_df_MA_KR %>% filter(Year == 2020)
combined_df_MA_KR_2020_SDnov <-
  combined_df_MA_KR_2020 %>% filter(SD == "01/11")

ggplot(combined_df_MA_KR_2020_SDnov , aes(x = NbJAS, y =Cstr,
                                          color= as.factor(Soil))) +
  geom_line( )+
  facet_grid(Variety~Mngt)+
  theme_minimal()


ggplot(combined_df_MA_KR_2020_SDnov, aes(x = NbJAS)) +
  geom_line(aes(y = Cstr, color = as.factor(Soil))) +
  geom_line(aes(y = SterilityCold, color = as.factor(Soil))) +
  geom_line(aes(y = GrainYieldPop/2000, color = as.factor(Soil)),
            linetype = "dashed") +
  facet_grid(Variety ~ Mngt) +
  theme_minimal()

#### moyenne sur stress hydrique pendant la saison de culture
combined_df_MA_KR_cropping_season <- combined_df_MA_KR %>%
  filter(NumPhase >= 1)


combined_df_MA_KR_2020_cropping_season <-
  combined_df_MA_KR_cropping_season %>% filter(Year == 2020)

combined_df_MA_KR_2020_SDnov_cropping_season <-
  combined_df_MA_KR_2020_cropping_season %>% filter(SD == "01/11")

ggplot(combined_df_MA_KR_2020_SDnov_cropping_season ,
       aes(x = NbJAS, y = SterilityCold,
                               color= as.factor(Variety))) +
  geom_line( )+
  facet_grid(combined_df_MA_KR_2020_SDnov_cropping_season$Soil
             ~combined_df_MA_KR_2020_SDnov_cropping_season$Mngt)

df_mean_MA_KR <- combined_df_MA_KR_cropping_season %>%
  group_by(Year,Variety,SD,Soil,Mngt) %>%
  summarize(mean_value_WS = mean(Cstr))

# Convert Category to a factor with the specified order
df_mean_MA_KR$SD<- factor(df_mean_MA_KR$SD, levels = category_order)
df_mean_MA_KR$Soil<- factor(df_mean_MA_KR$Soil, levels = category_order_soil)
df_mean_MA_KR$Mngt<- factor(df_mean_MA_KR$Mngt, levels = category_order_mngt)


ggplot(df_mean_MA_KR , aes(x = SD, y = mean_value_WS, color= Mngt)) +
  geom_boxplot() +
  labs(title = "Box Plot of Mean water stressp",
       x = "Group",
       y = "Mean Value") +
  facet_grid(df_mean_MA_KR$Variety~df_mean_MA_KR$Soil)+
  theme_minimal()

####selection sur grain filling period
combined_df_MA_KR_grain_filling<- combined_df_MA_KR %>%
  filter(NumPhase > 4)


combined_df_MA_KR_2020_grain_filling <-
  combined_df_MA_KR_grain_filling %>% filter(Year == 2020)

combined_df_MA_KR_2020_SDnov_grain_filling <-
  combined_df_MA_KR_2020_grain_filling %>% filter(SD == "01/11")

ggplot(combined_df_MA_KR_2020_SDnov_grain_filling ,
       aes(x = NbJAS, y = SterilityCold,
           color= as.factor(Variety))) +
  geom_line( )+
  facet_grid(combined_df_MA_KR_2020_SDnov_grain_filling$Soil
             ~combined_df_MA_KR_2020_SDnov_grain_filling$Mngt)


df_mean_MA_KR_grain_filling <- combined_df_MA_KR_grain_filling %>%
  group_by(Year,Variety,SD,Soil,Mngt) %>%
  summarize(mean_value_Cold_Stress = mean(SterilityCold))

# Convert Category to a factor with the specified order
df_mean_MA_KR_grain_filling$SD<- factor(df_mean_MA_KR_grain_filling$SD,
                                        levels = category_order)
df_mean_MA_KR_grain_filling$Soil<- factor(df_mean_MA_KR_grain_filling$Soil,
                                          levels = category_order_soil)
df_mean_MA_KR_grain_filling$Mngt<- factor(df_mean_MA_KR_grain_filling$Mngt,
                                          levels = category_order_mngt)


ggplot(df_mean_MA_KR_grain_filling ,
       aes(x = SD, y = mean_value_Cold_Stress, color= Mngt)) +
  geom_boxplot() +
  labs(title = "Box Plot of Mean cold sterility",
       x = "SD",
       y = "Mean Value") +
  facet_grid(df_mean_MA_KR_grain_filling$Variety~
               df_mean_MA_KR_grain_filling$Soil)+
  theme_minimal()

