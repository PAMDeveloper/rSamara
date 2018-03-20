#Install and load packages
list.of.packages <- c("rsamara","RODBC", "ggplot2", "gdata")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#detailled load
source('C:/Users/USER/Desktop/installation samara/rSamara/R/AccessLoader.R')
options(digits=18)
### load each info separated
connectDB('C:/Users/USER/Desktop/installation samara/Basedonnee_Malick.accdb')
###"simulation phéno SK5912 (G6)

.GlobalEnv$paramsSN14S1_G6 <- loadSimDetails('SINT2014S1', 'SK5912', 'SIN2_SOL', '4', '2014/07/07', '2014/12/20')
.GlobalEnv$meteoSN14S1G6 <- loadMeteo('4', '2014/07/07', '2014/12/20')
.GlobalEnv$obsSN14S1_G6<- loadObs('SINT2014S1G6', 'SK5912','2014/07/07', '2014/12/20')

.GlobalEnv$paramsSN14S2_G6 <- loadSimDetails('SINT2014S2', 'SK5912', 'SIN2_SOL', '4', '2014/07/27', '2014/12/30')
.GlobalEnv$meteoSN14S2G6 <- loadMeteo('4', '2014/07/27', '2014/12/30')
.GlobalEnv$obsSN14S2_G6<- loadObs('SINT2014S2G6', 'SK5912','2014/07/27', '2014/12/30')

.GlobalEnv$paramsSN16_G6 <- loadSimDetails('SINT2016S1', 'SK5912', 'SIN2_SOL', '4', '2016/07/15', '2016/12/20')
.GlobalEnv$meteoSN16G6 <- loadMeteo('4', '2016/07/15', '2016/12/20')
.GlobalEnv$obsSN16_G6<- loadObs('SINT2016S1G6', 'SK5912','2016/07/15', '2016/12/20')

.GlobalEnv$paramsSN13S1_G6 <- loadSimDetails('SINT2013S1', 'SK5912', 'SIN2_SOL', '4', '2013/07/15', '2013/12/20')
.GlobalEnv$meteoSN13S1G6 <- loadMeteo('4', '2013/07/15', '2013/12/20')
.GlobalEnv$obsSN13S1_G6 <- loadObs('SINT2013S1G6', 'SK5912','2013/07/15', '2013/12/20')

.GlobalEnv$paramsSN13S2_G6 <- loadSimDetails('SINT2013S2', 'SK5912', 'SIN2_SOL', '4', '2013/07/27', '2013/12/30')
.GlobalEnv$meteoSN13S2G6 <- loadMeteo('4', '2013/07/27', '2013/12/30')
.GlobalEnv$obsSN13S2_G6 <- loadObs('SINT2013S2G6', 'SK5912','2013/07/27', '2013/12/30')

.GlobalEnv$paramsSN15S1_G6 <- loadSimDetails('SINT2015S1', 'SK5912', 'SIN1_SOL', '4', '2015/06/29', '2015/12/20')
.GlobalEnv$meteoSN15S1G6 <- loadMeteo('4', '2015/06/29', '2015/12/20')
.GlobalEnv$obsSN15S1_G6<- loadObs('SINT2015S1G6', 'SK5912','2015/06/29', '2015/12/20')

.GlobalEnv$paramsSN15S2_G6 <- loadSimDetails('SINT2015S2', 'SK5912', 'SIN1_SOL', '4', '2015/07/29', '2015/12/30')
.GlobalEnv$meteoSN15S2G6 <- loadMeteo('4', '2015/07/29', '2015/12/30')
.GlobalEnv$obsSN15S2_G6<- loadObs('SINT2015S2G6', 'SK5912','2015/07/29', '2015/12/30')

# .GlobalEnv$paramsB13S1_G6 <- loadSimDetails('BBY2013S1', 'SK5912', 'BAM_SOL', '380084', '2013/07/07', '2013/12/20')
# .GlobalEnv$meteoB13S1G6 <- loadMeteo('380084', '2013/07/07', '2013/12/20')
# #.GlobalEnv$obsB13S1_G6<- loadObs('BBY2013S1G6', 'SK5912','2013/07/07', '2013/12/20')
# 
# .GlobalEnv$paramsB13S2_G6 <- loadSimDetails('BBY2013S2', 'SK5912', 'BAM_SOL', '380084', '2013/07/21', '2013/12/30')
# .GlobalEnv$meteoB13S2G6 <- loadMeteo('380084', '2013/07/21', '2013/12/30')
# #.GlobalEnv$obsB13S2_G6<- loadObs('BBY2013S2G6', 'SK5912','2013/07/21', '2013/12/30')
# 
# .GlobalEnv$paramsN15S1_G6 <- loadSimDetails('NIO2015S1', 'SK5912', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/20')
# .GlobalEnv$meteoN15S1G6 <- loadMeteo('330114', '2015/07/06', '2015/12/20')
# #.GlobalEnv$obsN15S1_G6<- loadObs('NIO2015S1G6', 'SK5912','2015/07/06', '2015/12/20')
# 
# .GlobalEnv$paramsN15S2_G6 <- loadSimDetails('NIO2015S2', 'SK5912', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/30')
# .GlobalEnv$meteoN15S2G6 <- loadMeteo('330114', '2015/08/03', '2015/12/30')
# #.GlobalEnv$obsN15S2_G6<- loadObs('NIO2015S2G6', 'SK5912','2015/08/03', '2015/12/30')

#### FADDA
.GlobalEnv$paramsSN14S1_G1 <- loadSimDetails('SINT2014S1', 'FADDA', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G1 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G1<- loadObs('SINT2014S1G1', 'FADDA','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G1 <- loadSimDetails('SINT2014S2', 'FADDA', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G1 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G1<- loadObs('SINT2014S2G1', 'FADDA','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G1 <- loadSimDetails('SINT2016S1', 'FADDA', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G1 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G1<- loadObs('SINT2016S1G1', 'FADDA','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G1 <- loadSimDetails('SINT2013S1', 'FADDA', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G1 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G1 <- loadObs('SINT2013S1G1', 'FADDA','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G1 <- loadSimDetails('SINT2013S2', 'FADDA', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G1 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G1 <- loadObs('SINT2013S2G1', 'FADDA','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G1 <- loadSimDetails('SINT2015S1', 'FADDA', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G1 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G1<- loadObs('SINT2015S1G1', 'FADDA','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G1 <- loadSimDetails('SINT2015S2', 'FADDA', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G1 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G1<- loadObs('SINT2015S2G9', 'FADDA','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G1 <- loadSimDetails('BBY2013S1', 'FADDA', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G1 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G1<- loadObs('BBY2013S1G1', 'FADDA','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G1 <- loadSimDetails('BBY2013S2', 'FADDA', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G1 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G1<- loadObs('BBY2013S2G9', 'FADDA','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G1 <- loadSimDetails('NIO2015S1', 'FADDA', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G1 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G1<- loadObs('NIO2015S1G9', '621B','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G1 <- loadSimDetails('NIO2015S2', 'FADDA', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G1 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G1<- loadObs('NIO2015S2G1', 'FADDA','2015/08/03', '2015/12/28')

#### NIELENI
.GlobalEnv$paramsSN14S1_G2 <- loadSimDetails('SINT2014S1', 'NIELENI', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G2 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G2<- loadObs('SINT2014S1G2', 'NIELENI','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G2 <- loadSimDetails('SINT2014S2', 'NIELENI', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G2 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G2<- loadObs('SINT2014S2G2', 'NIELENI','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G2 <- loadSimDetails('SINT2016S1', 'NIELENI', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G2 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G2<- loadObs('SINT2016S1G2', 'NIELENI','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G2 <- loadSimDetails('SINT2013S1', 'NIELENI', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G2 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G2 <- loadObs('SINT2013S1G2', 'NIELENI','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G2 <- loadSimDetails('SINT2013S2', 'NIELENI', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G2 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G2 <- loadObs('SINT2013S2G2', 'NIELENI','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G2 <- loadSimDetails('SINT2015S1', 'NIELENI', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G2 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G2<- loadObs('SINT2015S1G2', 'NIELENI','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G2 <- loadSimDetails('SINT2015S2', 'NIELENI', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G2 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G2<- loadObs('SINT2015S2G2', 'NIELENI','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G2 <- loadSimDetails('BBY2013S1', 'NIELENI', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G2 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G2<- loadObs('BBY2013S1G2', 'NIELENI','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G2 <- loadSimDetails('BBY2013S2', 'NIELENI', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G2 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G2<- loadObs('BBY2013S2G2', 'NIELENI','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G2 <- loadSimDetails('NIO2015S1', 'NIELENI', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G2 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G2<- loadObs('NIO2015S1G2', 'NIELENI','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G2 <- loadSimDetails('NIO2015S2', 'NIELENI', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G2 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G2<- loadObs('NIO2015S2G2', 'NIELENI','2015/08/03', '2015/12/28')

## IS15401

.GlobalEnv$paramsSN14S1_G3 <- loadSimDetails('SINT2014S1', 'IS15401', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G3 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G3<- loadObs('SINT2014S1G3', 'IS15401','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G3 <- loadSimDetails('SINT2014S2', 'IS15401', 'SIN2_SOL', '4', '2014/07/27', '2014/12/20')
.GlobalEnv$meteoSN14S2G3 <- loadMeteo('4', '2014/07/27', '2014/12/20')
.GlobalEnv$obsSN14S2_G3<- loadObs('SINT2014S2G3', 'IS15401','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G3 <- loadSimDetails('SINT2016S1', 'IS15401', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G3 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G3<- loadObs('SINT2016S1G3', 'IS15401','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G3 <- loadSimDetails('SINT2013S1', 'IS15401', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G3 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G3 <- loadObs('SINT2013S1G3', 'IS15401','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G3 <- loadSimDetails('SINT2013S2', 'IS15401', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G3 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G3<- loadObs('SINT2013S2G3', 'IS15401','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G3 <- loadSimDetails('SINT2015S1', 'IS15401', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G3 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G3<- loadObs('SINT2015S1G3', 'IS15401','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G3 <- loadSimDetails('SINT2015S2', 'IS15401', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G3 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G3<- loadObs('SINT2015S2G3', 'IS15401','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G3 <- loadSimDetails('BBY2013S1', 'IS15401', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G3 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G3<- loadObs('BBY2013S1G3', 'IS15401','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G3 <- loadSimDetails('BBY2013S2', 'IS15401', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G3 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G3<- loadObs('BBY2013S2G3', 'IS15401','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G3 <- loadSimDetails('NIO2015S1', 'IS15401', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G3 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G3<- loadObs('NIO2015S1G3', 'IS15401','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G3 <- loadSimDetails('NIO2015S2', 'IS15401', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G3 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G3<- loadObs('NIO2015S2G3', 'IS15401','2015/08/03', '2015/12/28')

## PABLO

.GlobalEnv$paramsSN14S1_G4 <- loadSimDetails('SINT2014S1', 'PABLO', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G4 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G4<- loadObs('SINT2014S1G4', 'PABLO','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G4 <- loadSimDetails('SINT2014S2', 'PABLO', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G4 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G4<- loadObs('SINT2014S2G4', 'PABLO','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G4 <- loadSimDetails('SINT2016S1', 'PABLO', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G4 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G4<- loadObs('SINT2016S1G4', 'PABLO','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G4 <- loadSimDetails('SINT2013S1', 'PABLO', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G4 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G4 <- loadObs('SINT2013S1G4', 'PABLO','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G4 <- loadSimDetails('SINT2013S2', 'PABLO', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G4 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G4 <- loadObs('SINT2013S2G4', 'PABLO','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G4 <- loadSimDetails('SINT2015S1', 'PABLO', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G4 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G4<- loadObs('SINT2015S1G4', 'PABLO','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G4 <- loadSimDetails('SINT2015S2', 'PABLO', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G4 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G4<- loadObs('SINT2015S2G4', 'PABLO','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G4 <- loadSimDetails('BBY2013S1', 'PABLO', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G4 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G4<- loadObs('BBY2013S1G4', 'PABLO','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G4 <- loadSimDetails('BBY2013S2', 'PABLO', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G4 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G4<- loadObs('BBY2013S2G4', 'PABLO','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G4 <- loadSimDetails('NIO2015S1', 'PABLO', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G4 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G4<- loadObs('NIO2015S1G4', 'PABLO','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G4 <- loadSimDetails('NIO2015S2', 'PABLO', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G4 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G4<- loadObs('NIO2015S2G4', 'PABLO','2015/08/03', '2015/12/28')

##CSM63E

.GlobalEnv$paramsSN14S1_G5 <- loadSimDetails('SINT2014S1', 'CSM63E', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G5 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G5<- loadObs('SINT2014S1G5', 'CSM63E','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G5 <- loadSimDetails('SINT2014S2', 'CSM63E', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G5 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G5<- loadObs('SINT2014S2G5', 'CSM63E','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G5 <- loadSimDetails('SINT2016S1', 'CSM63E', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G5 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G5<- loadObs('SINT2016S1G5', 'CSM63E','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G5 <- loadSimDetails('SINT2013S1', 'CSM63E', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G5 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G5 <- loadObs('SINT2013S1G5', 'CSM63E','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G5 <- loadSimDetails('SINT2013S2', 'CSM63E', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G5 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G5<- loadObs('SINT2013S2G5', 'CSM63E','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G5 <- loadSimDetails('SINT2015S1', 'CSM63E', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G5 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G5<- loadObs('SINT2015S1G5', 'CSM63E','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G5 <- loadSimDetails('SINT2015S2', 'CSM63E', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G5 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G5<- loadObs('SINT2015S2G5', 'CSM63E','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G5 <- loadSimDetails('BBY2013S1', 'CSM63E', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G5 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G5<- loadObs('BBY2013S1G5', 'CSM63E','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G5 <- loadSimDetails('BBY2013S2', 'CSM63E', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G5 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G5<- loadObs('BBY2013S2G5', 'CSM63E','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G5 <- loadSimDetails('NIO2015S1', 'CSM63E', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G5 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G5<- loadObs('NIO2015S1G5', 'CSM63E','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G5 <- loadSimDetails('NIO2015S2', 'CSM63E', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G5 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G5<- loadObs('NIO2015S2G5', 'CSM63E','2015/08/03', '2015/12/28')

##GRINKAN

## GRINKAN

.GlobalEnv$paramsSN14S1_G7 <- loadSimDetails('SINT2014S1', 'GRINKAN', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G7 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G7<- loadObs('SINT2014S1G7', 'GRINKAN','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G7 <- loadSimDetails('SINT2014S2', 'GRINKAN', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G7 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G7<- loadObs('SINT2014S2G7', 'GRINKAN','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G7 <- loadSimDetails('SINT2016S1', 'GRINKAN', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G7 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G7<- loadObs('SINT2016S1G7', 'GRINKAN','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G7 <- loadSimDetails('SINT2013S1', 'GRINKAN', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G7 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G7<- loadObs('SINT2013S1G7', 'GRINKAN','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G7 <- loadSimDetails('SINT2013S2', 'GRINKAN', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G7 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G7<- loadObs('SINT2013S2G7', 'GRINKAN','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G7 <- loadSimDetails('SINT2015S1', 'GRINKAN', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G7 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G7<- loadObs('SINT2015S1G7', 'GRINKAN','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G7 <- loadSimDetails('SINT2015S2', 'GRINKAN', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G7 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G7<- loadObs('SINT2015S2G7', 'GRINKAN','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G7 <- loadSimDetails('BBY2013S1', 'GRINKAN', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G7 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G7<- loadObs('BBY2013S1G7', 'GRINKAN','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G7 <- loadSimDetails('BBY2013S2', 'GRINKAN', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G7 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G7<- loadObs('BBY2013S2G7', 'GRINKAN','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G7 <- loadSimDetails('NIO2015S1', 'GRINKAN', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G7 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G7<- loadObs('NIO2015S1G7', 'GRINKAN','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G7 <- loadSimDetails('NIO2015S2', 'GRINKAN', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G7 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G7<- loadObs('NIO2015S2G7', 'GRINKAN','2015/08/03', '2015/12/28')

##SOUMBA

.GlobalEnv$paramsSN14S1_G8 <- loadSimDetails('SINT2014S1', 'SOUMBA', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G8 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G8<- loadObs('SINT2014S1G8', 'SOUMBA','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G8 <- loadSimDetails('SINT2014S2', 'SOUMBA', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G8 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G8<- loadObs('SINT2014S2G8', 'SOUMBA','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G8 <- loadSimDetails('SINT2016S1', 'SOUMBA', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G8 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G8<- loadObs('SINT2016S1G8', 'SOUMBA','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G8 <- loadSimDetails('SINT2013S1', 'SOUMBA', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G8 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G8<- loadObs('SINT2013S1G8', 'SOUMBA','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G8 <- loadSimDetails('SINT2013S2', 'SOUMBA', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G8 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G8<- loadObs('SINT2013S2G8', 'SOUMBA','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G8 <- loadSimDetails('SINT2015S1', 'SOUMBA', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G8 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G8<- loadObs('SINT2015S1G8', 'SOUMBA','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G8 <- loadSimDetails('SINT2015S2', 'SOUMBA', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G8 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G8<- loadObs('SINT2015S2G8', 'SOUMBA','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G8 <- loadSimDetails('BBY2013S1', 'SOUMBA', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G8 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G8<- loadObs('BBY2013S1G8', 'SOUMBA','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G8 <- loadSimDetails('BBY2013S2', 'SOUMBA', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G8 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G8<- loadObs('BBY2013S2G8', 'SOUMBA','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G8 <- loadSimDetails('NIO2015S1', 'SOUMBA', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G8 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G8<- loadObs('NIO2015S1G8', 'SOUMBA','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G8 <- loadSimDetails('NIO2015S2', 'SOUMBA', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G8 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G8<- loadObs('NIO2015S2G8', 'SOUMBA','2015/08/03', '2015/12/28')

### 621B

.GlobalEnv$paramsSN14S1_G9 <- loadSimDetails('SINT2014S1', '621B', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G9 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G9<- loadObs('SINT2014S1G9', '621B','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G9 <- loadSimDetails('SINT2014S2', '621B', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G9 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G9<- loadObs('SINT2014S2G9', '621B','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G9 <- loadSimDetails('SINT2016S1', '621B', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G9 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G9<- loadObs('SINT2016S1G9', '621B','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G9 <- loadSimDetails('SINT2013S1', '621B', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G9 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G9<- loadObs('SINT2013S1G9', '621B','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G9 <- loadSimDetails('SINT2013S2', '621B', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G9 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G9<- loadObs('SINT2013S2G9', '621B','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G9 <- loadSimDetails('SINT2015S1', '621B', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G9 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G9<- loadObs('SINT2015S1G9', '621B','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G9 <- loadSimDetails('SINT2015S2', '621B', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G9 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G9<- loadObs('SINT2015S2G9', '621B','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G9 <- loadSimDetails('BBY2013S1', '621B', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G9 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G9<- loadObs('BBY2013S1G9', '621B','2013/07/07', '2013/12/03')
#
# .GlobalEnv$paramsB13S2_G9 <- loadSimDetails('BBY2013S2', '621B', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G9 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G9<- loadObs('BBY2013S2G9', '621B','2013/07/21', '2013/12/03')
#
# .GlobalEnv$paramsN15S1_G9 <- loadSimDetails('NIO2015S1', '621B', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G9 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G9<- loadObs('NIO2015S1G9', '621B','2015/07/06', '2015/12/26')
#
# .GlobalEnv$paramsN15S2_G9 <- loadSimDetails('NIO2015S2', '621B', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G9 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G9<- loadObs('NIO2015S2G9', '621B','2015/08/03', '2015/12/28')


### F2-20

.GlobalEnv$paramsSN14S1_G10 <- loadSimDetails('SINT2014S1', 'LOCAL', 'SIN2_SOL', '4', '2014/07/07', '2014/12/06')
.GlobalEnv$meteoSN14S1G10 <- loadMeteo('4', '2014/07/07', '2014/12/06')
.GlobalEnv$obsSN14S1_G10<- loadObs('SINT2014S1G10', 'LOCAL','2014/07/07', '2014/12/06')

.GlobalEnv$paramsSN14S2_G10 <- loadSimDetails('SINT2014S2', 'LOCAL', 'SIN2_SOL', '4', '2014/07/27', '2014/12/05')
.GlobalEnv$meteoSN14S2G10 <- loadMeteo('4', '2014/07/27', '2014/12/05')
.GlobalEnv$obsSN14S2_G10<- loadObs('SINT2014S2G10', 'LOCAL','2014/07/27', '2014/12/05')

.GlobalEnv$paramsSN16_G10 <- loadSimDetails('SINT2016S1', 'LOCAL', 'SIN2_SOL', '4', '2016/07/15', '2016/12/06')
.GlobalEnv$meteoSN16G10 <- loadMeteo('4', '2016/07/15', '2016/12/06')
.GlobalEnv$obsSN16_G10<- loadObs('SINT2016S1G10', 'LOCAL','2016/07/15', '2016/12/06')

.GlobalEnv$paramsSN13S1_G10 <- loadSimDetails('SINT2013S1', 'LOCAL', 'SIN2_SOL', '4', '2013/07/15', '2013/12/10')
.GlobalEnv$meteoSN13S1G10 <- loadMeteo('4', '2013/07/15', '2013/12/10')
.GlobalEnv$obsSN13S1_G10<- loadObs('SINT2013S1G10', 'LOCAL','2013/07/15', '2013/12/10')

.GlobalEnv$paramsSN13S2_G10 <- loadSimDetails('SINT2013S2', 'LOCAL', 'SIN2_SOL', '4', '2013/07/27', '2013/12/10')
.GlobalEnv$meteoSN13S2G10 <- loadMeteo('4', '2013/07/27', '2013/12/10')
.GlobalEnv$obsSN13S2_G10<- loadObs('SINT2013S2G10', 'LOCAL','2013/07/27', '2013/12/10')

.GlobalEnv$paramsSN15S1_G10 <- loadSimDetails('SINT2015S1', 'LOCAL', 'SIN1_SOL', '4', '2015/06/29', '2015/12/07')
.GlobalEnv$meteoSN15S1G10 <- loadMeteo('4', '2015/06/29', '2015/12/07')
.GlobalEnv$obsSN15S1_G10<- loadObs('SINT2015S1G10', 'LOCAL','2015/06/29', '2015/12/07')

.GlobalEnv$paramsSN15S2_G10 <- loadSimDetails('SINT2015S2', 'LOCAL', 'SIN1_SOL', '4', '2015/07/29', '2015/12/18')
.GlobalEnv$meteoSN15S2G10 <- loadMeteo('4', '2015/07/29', '2015/12/18')
.GlobalEnv$obsSN15S2_G10<- loadObs('SINT2015S2G10', 'LOCAL','2015/07/29', '2015/12/18')

# .GlobalEnv$paramsB13S1_G10 <- loadSimDetails('BBY2013S1', 'LOCAL', 'BAM_SOL', '380084', '2013/07/07', '2013/12/03')
# .GlobalEnv$meteoB13S1G10 <- loadMeteo('380084', '2013/07/07', '2013/12/03')
# #.GlobalEnv$obsB13S1_G10<- loadObs('BBY2013S1G10', 'LOCAL','2013/07/07', '2013/12/03')
# 
# .GlobalEnv$paramsB13S2_G10 <- loadSimDetails('BBY2013S2', 'LOCAL', 'BAM_SOL', '380084', '2013/07/21', '2013/12/03')
# .GlobalEnv$meteoB13S2G10 <- loadMeteo('380084', '2013/07/21', '2013/12/03')
# #.GlobalEnv$obsB13S2_G10<- loadObs('BBY2013S2G10', 'LOCAL','2013/07/21', '2013/12/03')
# 
# .GlobalEnv$paramsN15S1_G10 <- loadSimDetails('NIO2015S1', 'LOCAL', 'NIO1_SOL', '330114', '2015/07/06', '2015/12/26')
# .GlobalEnv$meteoN15S1G10 <- loadMeteo('330114', '2015/07/06', '2015/12/26')
# #.GlobalEnv$obsN15S1_G10<- loadObs('NIO2015S1G10', 'LOCAL','2015/07/06', '2015/12/26')
# 
# .GlobalEnv$paramsN15S2_G10 <- loadSimDetails('NIO2015S2', 'LOCAL', 'NIO2_SOL', '330114', '2015/08/03', '2015/12/28')
# .GlobalEnv$meteoN15S2G10 <- loadMeteo('330114', '2015/08/03', '2015/12/28')
# #.GlobalEnv$obsN15S2_G10<- loadObs('NIO2015S2G10', 'LOCAL','2015/08/03', '2015/12/28')


# #Boucle de simulations depuis un fichier Xls
#  connectDB('C:/Users/USER/Desktop/installation samara/Basedonnee_Malick.accdb')
#  perl="C:/Users/USER/Desktop/installation samara/Perl/bin/perl.exe"
#  simulist = read.xls("C:/Users/USER/Desktop/installation samara/rSamara/simulation.xlsx", perl=perl)
#   launchFromList <- function (simcode, itkcode, variety, plotcode, wscode, startingdate, endingdate) {
#     print(simcode)
#     meteoxls <- loadMeteo(wscode,startingdate,endingdate)
#     paramsXls = loadSimDetails(itkcode,variety,plotcode,wscode,startingdate,endingdate)
#     xlsresults = rsamara::run2DF(paramsXls,meteoxls)
#     filename = paste(simcode, ".csv")
#     write.table(format(xlsresults, digits=18), filename, sep='\t',row.names=F)
#     print(paste(simcode, " written"))
#   }
#   mapply(launchFromList, simulist$simcode, simulist$itkcode, simulist$variety, simulist$plotcode, simulist$wscode, simulist$startingdate, simulist$endingdate)

 ### load from sim Table
 # initSim('D:/PAMStudio_dev/git/Basedonnee_Malick.accdb', '06SB15-fev13-D1_SV21')

 odbcCloseAll()## ferme la base de donnée
 
#### résultats SAMARA 
 ##SK5912 (G6)
 results_SamaraSN14S1_G6 =rsamara::run2DF(paramsSN14S1_G6,meteoSN14S1G6)
 results_SamaraSN14S2_G6 =rsamara::run2DF(paramsSN14S2_G6,meteoSN14S2G6)
 results_SamaraSN16_G6 =rsamara::run2DF(paramsSN16_G6,meteoSN16G6)
 results_SamaraSN13S1_G6=rsamara::run2DF(paramsSN13S1_G6,meteoSN13S1G6)
 results_SamaraSN13S2_G6 =rsamara::run2DF(paramsSN13S2_G6,meteoSN13S2G6)
 results_SamaraSN15S1_G6 =rsamara::run2DF(paramsSN15S1_G6,meteoSN15S1G6)
 results_SamaraSN15S2_G6 =rsamara::run2DF(paramsSN15S2_G6,meteoSN15S2G6)
 # results_SamaraB13S1_G6 =rsamara::run2DF(paramsB13S1_G6,meteoB13S1G6)
 # results_SamaraB13S2_G6 =rsamara::run2DF(paramsB13S2_G6,meteoSB13S2G6)
 # results_SamaraN15S1_G6 =rsamara::run2DF(paramsN15S1_G6,meteoN15S1G6)
 # results_SamaraN15S2_G6 =rsamara::run2DF(paramsN15S2_G6,meteoN15S2G6)
 ## FADDA
 results_SamaraSN14S1_G1 =rsamara::run2DF(paramsSN14S1_G1,meteoSN14S1G1)
 results_SamaraSN14S2_G1 =rsamara::run2DF(paramsSN14S2_G1,meteoSN14S2G1)
 results_SamaraSN16_G1 =rsamara::run2DF(paramsSN16_G1,meteoSN16G1)
 results_SamaraSN13S1_G1=rsamara::run2DF(paramsSN13S1_G1,meteoSN13S1G1)
 results_SamaraSN13S2_G1 =rsamara::run2DF(paramsSN13S2_G1,meteoSN13S2G1)
 results_SamaraSN15S1_G1 =rsamara::run2DF(paramsSN15S1_G1,meteoSN15S1G1)
 results_SamaraSN15S2_G1 =rsamara::run2DF(paramsSN15S2_G1,meteoSN15S2G1)
 # results_SamaraB13S1_G1 =rsamara::run2DF(paramsB13S1_G1,meteoB13S1G1)
 # results_SamaraB13S2_G1 =rsamara::run2DF(paramsB13S2_G1,meteoSB13S2G1)
 # results_SamaraN15S1_G1 =rsamara::run2DF(paramsN15S1_G1,meteoN15S1G1)
 # results_SamaraN15S2_G1 =rsamara::run2DF(paramsN15S2_G1,meteoN15S2G1)
##Ù figure avec nombre de jours aprés semis
 
 ##NIELENI
 
 results_SamaraSN14S1_G2 =rsamara::run2DF(paramsSN14S1_G2,meteoSN14S1G2)
 results_SamaraSN14S2_G2 =rsamara::run2DF(paramsSN14S2_G2,meteoSN14S2G2)
 results_SamaraSN16_G2 =rsamara::run2DF(paramsSN16_G2,meteoSN16G2)
 results_SamaraSN13S1_G2=rsamara::run2DF(paramsSN13S1_G2,meteoSN13S1G2)
 results_SamaraSN13S2_G2 =rsamara::run2DF(paramsSN13S2_G2,meteoSN13S2G2)
 results_SamaraSN15S1_G2 =rsamara::run2DF(paramsSN15S1_G2,meteoSN15S1G2)
 results_SamaraSN15S2_G2 =rsamara::run2DF(paramsSN15S2_G2,meteoSN15S2G2)
 # results_SamaraB13S1_G2 =rsamara::run2DF(paramsB13S1_G2,meteoB13S1G2)
 # results_SamaraB13S2_G2 =rsamara::run2DF(paramsB13S2_G2,meteoSB13S2G2)
 # results_SamaraN15S1_G2 =rsamara::run2DF(paramsN15S1_G2,meteoN15S1G2)
 # results_SamaraN15S2_G2 =rsamara::run2DF(paramsN15S2_G2,meteoN15S2G2)
 
 ### IS15401
 
 results_SamaraSN14S1_G3 =rsamara::run2DF(paramsSN14S1_G3,meteoSN14S1G3)
 results_SamaraSN14S2_G3 =rsamara::run2DF(paramsSN14S2_G3,meteoSN14S2G3)
 results_SamaraSN16_G3 =rsamara::run2DF(paramsSN16_G3,meteoSN16G3)
 results_SamaraSN13S1_G3=rsamara::run2DF(paramsSN13S1_G3,meteoSN13S1G3)
 results_SamaraSN13S2_G3 =rsamara::run2DF(paramsSN13S2_G3,meteoSN13S2G3)
 results_SamaraSN15S1_G3 =rsamara::run2DF(paramsSN15S1_G3,meteoSN15S1G3)
 results_SamaraSN15S2_G3 =rsamara::run2DF(paramsSN15S2_G3,meteoSN15S2G3)
 # results_SamaraB13S1_G3 =rsamara::run2DF(paramsB13S1_G3,meteoB13S1G3)
 # results_SamaraB13S2_G3 =rsamara::run2DF(paramsB13S2_G3,meteoSB13S2G3)
 # results_SamaraN15S1_G3 =rsamara::run2DF(paramsN15S1_G3,meteoN15S1G3)
 # results_SamaraN15S2_G3 =rsamara::run2DF(paramsN15S2_G3,meteoN15S2G3)
 
 ### PABLO
 
 results_SamaraSN14S1_G4 =rsamara::run2DF(paramsSN14S1_G4,meteoSN14S1G4)
 results_SamaraSN14S2_G4 =rsamara::run2DF(paramsSN14S2_G4,meteoSN14S2G4)
 results_SamaraSN16_G4 =rsamara::run2DF(paramsSN16_G4,meteoSN16G4)
 results_SamaraSN13S1_G4=rsamara::run2DF(paramsSN13S1_G4,meteoSN13S1G4)
 results_SamaraSN13S2_G4 =rsamara::run2DF(paramsSN13S2_G4,meteoSN13S2G4)
 results_SamaraSN15S1_G4 =rsamara::run2DF(paramsSN15S1_G4,meteoSN15S1G4)
 results_SamaraSN15S2_G4 =rsamara::run2DF(paramsSN15S2_G4,meteoSN15S2G4)
 # results_SamaraB13S1_G4 =rsamara::run2DF(paramsB13S1_G4,meteoB13S1G4)
 # results_SamaraB13S2_G4 =rsamara::run2DF(paramsB13S2_G4,meteoSB13S2G4)
 # results_SamaraN15S1_G4 =rsamara::run2DF(paramsN15S1_G4,meteoN15S1G4)
 # results_SamaraN15S2_G4 =rsamara::run2DF(paramsN15S2_G4,meteoN15S2G4) 
 
 ## CSM63E
 
 results_SamaraSN14S1_G5 =rsamara::run2DF(paramsSN14S1_G5,meteoSN14S1G5)
 results_SamaraSN14S2_G5 =rsamara::run2DF(paramsSN14S2_G5,meteoSN14S2G5)
 results_SamaraSN16_G5 =rsamara::run2DF(paramsSN16_G5,meteoSN16G5)
 results_SamaraSN13S1_G5=rsamara::run2DF(paramsSN13S1_G5,meteoSN13S1G5)
 results_SamaraSN13S2_G5 =rsamara::run2DF(paramsSN13S2_G5,meteoSN13S2G5)
 results_SamaraSN15S1_G5 =rsamara::run2DF(paramsSN15S1_G5,meteoSN15S1G5)
 results_SamaraSN15S2_G5 =rsamara::run2DF(paramsSN15S2_G5,meteoSN15S2G5)
 # results_SamaraB13S1_G5 =rsamara::run2DF(paramsB13S1_G5,meteoB13S1G5)
 # results_SamaraB13S2_G5 =rsamara::run2DF(paramsB13S2_G5,meteoSB13S2G5)
 # results_SamaraN15S1_G5 =rsamara::run2DF(paramsN15S1_G5,meteoN15S1G5)
 # results_SamaraN15S2_G5 =rsamara::run2DF(paramsN15S2_G5,meteoN15S2G5)
 
 ### GRINKAN
 
 results_SamaraSN14S1_G7 =rsamara::run2DF(paramsSN14S1_G7,meteoSN14S1G7)
 results_SamaraSN14S2_G7 =rsamara::run2DF(paramsSN14S2_G7,meteoSN14S2G7)
 results_SamaraSN16_G7 =rsamara::run2DF(paramsSN16_G7,meteoSN16G7)
 results_SamaraSN13S1_G7=rsamara::run2DF(paramsSN13S1_G7,meteoSN13S1G7)
 results_SamaraSN13S2_G7 =rsamara::run2DF(paramsSN13S2_G7,meteoSN13S2G7)
 results_SamaraSN15S1_G7 =rsamara::run2DF(paramsSN15S1_G7,meteoSN15S1G7)
 results_SamaraSN15S2_G7 =rsamara::run2DF(paramsSN15S2_G7,meteoSN15S2G7)
 # results_SamaraB13S1_G7 =rsamara::run2DF(paramsB13S1_G7,meteoB13S1G7)
 # results_SamaraB13S2_G7 =rsamara::run2DF(paramsB13S2_G7,meteoSB13S2G7)
 # results_SamaraN15S1_G7 =rsamara::run2DF(paramsN15S1_G7,meteoN15S1G7)
 # results_SamaraN15S2_G7 =rsamara::run2DF(paramsN15S2_G7,meteoN15S2G7)
 
 ### SOUMBA
 
 results_SamaraSN14S1_G8 =rsamara::run2DF(paramsSN14S1_G8,meteoSN14S1G8)
 results_SamaraSN14S2_G8 =rsamara::run2DF(paramsSN14S2_G8,meteoSN14S2G8)
 results_SamaraSN16_G8 =rsamara::run2DF(paramsSN16_G8,meteoSN16G8)
 results_SamaraSN13S1_G8=rsamara::run2DF(paramsSN13S1_G8,meteoSN13S1G8)
 results_SamaraSN13S2_G8 =rsamara::run2DF(paramsSN13S2_G8,meteoSN13S2G8)
 results_SamaraSN15S1_G8 =rsamara::run2DF(paramsSN15S1_G8,meteoSN15S1G8)
 results_SamaraSN15S2_G8 =rsamara::run2DF(paramsSN15S2_G8,meteoSN15S2G8)
 # results_SamaraB13S1_G8 =rsamara::run2DF(paramsB13S1_G8,meteoB13S1G8)
 # results_SamaraB13S2_G8 =rsamara::run2DF(paramsB13S2_G8,meteoSB13S2G8)
 # results_SamaraN15S1_G8 =rsamara::run2DF(paramsN15S1_G8,meteoN15S1G8)
 # results_SamaraN15S2_G8 =rsamara::run2DF(paramsN15S2_G8,meteoN15S2G8)
 
 ### 621B
 results_SamaraSN14S1_G9 =rsamara::run2DF(paramsSN14S1_G9,meteoSN14S1G9)
 results_SamaraSN14S2_G9 =rsamara::run2DF(paramsSN14S2_G9,meteoSN14S2G9)
 results_SamaraSN16_G9 =rsamara::run2DF(paramsSN16_G9,meteoSN16G9)
 results_SamaraSN13S1_G9=rsamara::run2DF(paramsSN13S1_G9,meteoSN13S1G9)
 results_SamaraSN13S2_G9 =rsamara::run2DF(paramsSN13S2_G9,meteoSN13S2G9)
 results_SamaraSN15S1_G9 =rsamara::run2DF(paramsSN15S1_G9,meteoSN15S1G9)
 results_SamaraSN15S2_G9 =rsamara::run2DF(paramsSN15S2_G9,meteoSN15S2G9)
 # results_SamaraB13S1_G9 =rsamara::run2DF(paramsB13S1_G9,meteoB13S1G9)
 # results_SamaraB13S2_G9 =rsamara::run2DF(paramsB13S2_G9,meteoSB13S2G9)
 # results_SamaraN15S1_G9 =rsamara::run2DF(paramsN15S1_G9,meteoN15S1G9)
 # results_SamaraN15S2_G9 =rsamara::run2DF(paramsN15S2_G9,meteoN15S2G9) 
 
 ### LOCAL
 
 results_SamaraSN14S1_G10 =rsamara::run2DF(paramsSN14S1_G10,meteoSN14S1G10)
 results_SamaraSN14S2_G10 =rsamara::run2DF(paramsSN14S2_G10,meteoSN14S2G10)
 results_SamaraSN16_G10 =rsamara::run2DF(paramsSN16_G10,meteoSN16G10)
 results_SamaraSN13S1_G10=rsamara::run2DF(paramsSN13S1_G10,meteoSN13S1G10)
 results_SamaraSN13S2_G10 =rsamara::run2DF(paramsSN13S2_G10,meteoSN13S2G10)
 results_SamaraSN15S1_G10 =rsamara::run2DF(paramsSN15S1_G10,meteoSN15S1G10)
 results_SamaraSN15S2_G10 =rsamara::run2DF(paramsSN15S2_G10,meteoSN15S2G10)
 # results_SamaraB13S1_G10 =rsamara::run2DF(paramsB13S1_G10,meteoB13S1G10)
 # results_SamaraB13S2_G10 =rsamara::run2DF(paramsB13S2_G10,meteoSB13S2G10)
 # results_SamaraN15S1_G10 =rsamara::run2DF(paramsN15S1_G10,meteoN15S1G10)
 # results_SamaraN15S2_G10 =rsamara::run2DF(paramsN15S2_G10,meteoN15S2G10)
 
 
 #### FIGURE AVEC NOMBRE JOURS APRES SEMIS
 ?par
 ## FADDA
 
 ggplot()+
   geom_point( data = obsSN14S1_G1, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G1, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G1, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G1, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G1, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G1, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G1, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G1, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G1, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G1, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G1, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G1, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G1, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G1, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G1, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G1, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G1, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G1, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G1, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G1, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G1, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G1, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_FADDA") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## NIELENI
 
 ggplot()+
   geom_point( data = obsSN14S1_G2, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G2, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G2, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G2, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G2, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G2, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G2, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G2, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G2, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G2, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G2, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G2, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G2, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G2, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G2, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G2, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G2, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G2, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G2, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G2, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G2, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G2, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_NIELENI") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## IS15401
 
 ggplot()+
   geom_point( data = obsSN14S1_G3, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G3, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G3, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G3, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G3, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G3, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G3, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G3, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G3, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G3, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G3, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G3, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G3, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G3, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G3, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G3, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G3, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G3, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G3, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G3, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G3, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G3, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_IS15401") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## PABLO
 
 ggplot()+
   geom_point( data = obsSN14S1_G4, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G4, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G4, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G4, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G4, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G4, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G4, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G4, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G4, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G4, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G4, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G4, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G4, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G4, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G4, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G4, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G4, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G4, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G4, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G4, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G4, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G4, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_PABLO") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## CSM63E
 
 ggplot()+
   geom_point( data = obsSN14S1_G5, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G5, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G5, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G5, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G5, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G5, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G5, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G5, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G5, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G5, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G5, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G5, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G5, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G5, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G5, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G5, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G5, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G5, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G5, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G5, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G5, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G5, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_CSM63E") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 
 ## SK5912
 
 ggplot()+
   geom_point( data = obsSN14S1_G6, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G6, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G6, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G6, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G6, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G6, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G6, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G6, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G6, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G6, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G6, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G6, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G6, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G6, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G6, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G6, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G6, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G6, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G6, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G6, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G6, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G6, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_SK5912") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## GRINKAN
 
 ggplot()+
   geom_point( data = obsSN14S1_G7, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G7, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G7, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G7, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G7, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G7, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G7, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G7, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G7, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G7, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G7, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G7, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G7, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G7, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G7, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G7, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G7, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G7, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G7, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G7, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G7, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G7, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_GRINKAN") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## SOUMBA
 
 ggplot()+
   geom_point( data = obsSN14S1_G8, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G8, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G8, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G8, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G8, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G8, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G8, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G8, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G8, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G8, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G8, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G8, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G8, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G8, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G8, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G8, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G8, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G8, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G8, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G8, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G8, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G8, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_SOUMBA") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 
 ## 621B
 
 ggplot()+
   geom_point( data = obsSN14S1_G9, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G9, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G9, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G9, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G9, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G9, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G9, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G9, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G9, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G9, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G9, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G9, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G9, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G9, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G9, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G9, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G9, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G9, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G9, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G9, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G9, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G9, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_621B") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 
 ## F2-20
 
 ggplot()+
   geom_point( data = obsSN14S1_G10, aes( x=nbjas, y =NumPhase, color = 'SN14S1'))+
   geom_point( data = obsSN14S2_G10, aes( x=nbjas, y =NumPhase, color = 'SN14S2'))+
   geom_point( data = obsSN16_G10, aes( x=nbjas, y =NumPhase, color = 'SN16'))+
   # geom_point( data = obsSN13S1_G10, aes(x =nbjas, y =NumPhase , color = 'SN13S1'))+
   # geom_point( data = obsSN13S2_G10, aes(x =nbjas, y =NumPhase, color = 'SN13S2'))+
   # geom_point( data = obsSN15S1_G10, aes( x=nbjas, y =NumPhase, color = 'SN15S1'))+
   # geom_point( data = obsSN15S2_G10, aes( x=nbjas, y =NumPhase, color = 'SN15S2'))+
   # geom_point( data = obsB13S1_G10, aes( x=nbjas, y =NumPhase, color = 'B13S1'))+
   # geom_point( data = obsB13S2_G10, aes( x=nbjas, y =NumPhase, color = 'B13S2'))+
   # geom_point( data = obsN15S1_G10, aes( x=nbjas, y =NumPhase, color = 'N15S1'))+
   # geom_point( data = obsN15S2_G10, aes( x=nbjas, y =NumPhase, color = 'N15S2'))+
   geom_line(data =  results_SamaraSN14S1_G10, aes(x = NbJas, y = NumPhase,color= "SN14S1"))+
   geom_line(data =  results_SamaraSN14S2_G10, aes(x = NbJas, y = NumPhase,color= "SN14S2"))+
   geom_line(data =  results_SamaraSN16_G10, aes(x = NbJas, y = NumPhase,color= "SN16"))+
   # geom_line(data =  results_SamaraSN13S1_G10, aes(x = NbJas, y = NumPhase, color= "SN13S1"))+
   # geom_line(data =  results_SamaraSN13S2_G10, aes(x = NbJas, y = NumPhase, color= "SN13S2"))+
   # geom_line(data =  results_SamaraSN15S1_G10, aes(x = NbJas, y = NumPhase,color= "SN15S1"))+
   # geom_line(data =  results_SamaraSN15S2_G10, aes(x = NbJas, y = NumPhase,color= "SN15S2"))+
   # # geom_line(data =  results_SamaraB13S1_G10, aes(x = NbJas, y = NumPhase,color= "B13S1"))+
   # geom_line(data =  results_SamaraB13S2_G10, aes(x = NbJas, y = NumPhase,color= "B13S2"))+
   # geom_line(data =  results_SamaraN15S1_G10, aes(x = NbJas, y = NumPhase,color= "N15S1"))+
   # geom_line(data =  results_SamaraN15S2_G10, aes(x = NbJas, y = NumPhase,color= "N15S2"))+
   ggtitle("Phénologie_F2-20") +
   ylab("") + xlab("Jour aprés semis")+
   theme(plot.title = element_text(lineheight=.8, face="bold"))
 

 ### comparaison degrés jour
 # results_SamaraSN16_G6$degresdujour
 # ggplot()+
 #   geom_line(data =  results_SamaraSN13S1_G1, aes(x = NbJas, y =degresdujour , color= "SN13S1"))+
 #   geom_line(data =  results_SamaraSN16_G1, aes(x = NbJas, y = degresdujour,color= "SN16"))
 # 

 # ggplot()+
 #   #geom_point(data = obs, aes(x = NbJas, y = lai))+
 #   geom_point( data = obsSN16_G1, aes(x =nbnbjas, y =lai, color = 'SN13S1'))+
 #   geom_point( data = obsSN16_G1, aes(x =nbnbjas, y =lai, color = 'SN13S2'))+
 #   geom_point( data = obsSN16_G1, aes(x =nbnbjas, y =lai, color = 'SN16'))+
 #   #geom_line(data =  results_simul_test, aes(x = NbJas, y = lai))+
 #   geom_line(data =  results_SamaraSN13S1_G1, aes(x = Nbnbjas, y = Lai, color= "SN13S1"))+
 #   geom_line(data =  results_SamaraSN13S2_G1, aes(x = Nbnbjas, y = Lai, color= "SN13S2"))+
 #   geom_line(data =  results_SamaraSN16_G1, aes(x = Nbnbjas, y = Lai,color= "SN16"))+
 #   ggtitle("Phenology") +
 #   ylab("") + xlab("Days after sowing")+
 #   theme(plot.title = element_text(lineheight=.8, face="bold"))

# param = rbind (paramsBama14S1, paramsSN13S1)
# write.table(format(param, digits=18), 'D:/PAMStudio_dev/git/rSamara/param.csv', sep='\t',row.names=F)
# 
# 
# METEO = rbind (meteoBama14S1, meteoSN13S1)
# write.table(format(METEO, digits=18), 'D:/PAMStudio_dev/git/rSamara/meteo.csv', sep='\t',row.names=F)


 ## Write Results FADDA
  write.table(format(results_SamaraSN14S1_G1, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G1.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G1, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G1.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G1, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G1.csv', sep='\t',row.names=F)
  ## Write Results NIELENI
  write.table(format(results_SamaraSN14S1_G2, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G2.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G2, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G2.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G2, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G2.csv', sep='\t',row.names=F)
  
  ## Write Results IS15401
  write.table(format(results_SamaraSN14S1_G3, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G3.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G3, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G3.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G3, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G3.csv', sep='\t',row.names=F)
  
  ## Write Results PABLO
  write.table(format(results_SamaraSN14S1_G4, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G4.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G4, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G4.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G4, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G4.csv', sep='\t',row.names=F)
  
  ## Write Results CSM63E
  write.table(format(results_SamaraSN14S1_G5, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G5.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G5, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G5.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G5, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G5.csv', sep='\t',row.names=F)
  
  ## Write Results SK5912
  write.table(format(results_SamaraSN14S1_G6, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G6.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G6, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G6.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G6, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G6.csv', sep='\t',row.names=F)
  
  ## Write Results GRINKAN
  write.table(format(results_SamaraSN14S1_G7, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G7.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G7, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G7.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G7, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G7.csv', sep='\t',row.names=F)
  
  ## Write Results SOUMBA
  write.table(format(results_SamaraSN14S1_G8, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G8.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G8, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G8.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G8, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G8.csv', sep='\t',row.names=F)
  
  ## Write Results 621B
  write.table(format(results_SamaraSN14S1_G9, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G9.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G9, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G9.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G9, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G9.csv', sep='\t',row.names=F)
  
  ## Write Results F2_20
  write.table(format(results_SamaraSN14S1_G10, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S1_G10.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN14S2_G10, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN14S2_G10.csv', sep='\t',row.names=F)
  write.table(format(results_SamaraSN16_G10, digits=18), 'C:/Users/USER/Desktop/installation samara/rSamara/Res_SN16S1_G10.csv', sep='\t',row.names=F)
  
  
   ## Plot all results
 # allPlot <- function() {
 #   plotF <- function(x) {
 #     plot(results_Samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(1.3*min(results_Samara[[x]], na.rm=T),1.3*max(results_Samara[[x]], na.rm=T)))
 #     return("Done")
 #   }
 # 
 #   sapply(names(results_Samara), plotF)
 # }
 # allPlot()


