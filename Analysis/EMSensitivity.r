#Script sensitivity analysis for EcoMeristem
#Authors : Florian Larue, Gr?gory Beurier, Jean-Christophe Soulie, Lauriane Rouan
#(PAM, AGAP, BIOS, CIRAD)

#####################Gestion des entr?es-sorties#####################
OVariable <- "drymatstructinternodepop"             #nom variable ? observer
Method <- "M"                         #m?thode AS (M = morris, C = complet, F = fast)
ValParamName <- "ValParam.txt"
PPath <- "D:/PAMStudio/dev/git/rSamara/Analysis/"
##########################################


#Install and load packages
list.of.packages <- c("rsamara","RODBC", "sensitivity")
new.packages <- list.of.packages[!(list.of.packages %in% installed.packages()[,"Package"])]
if(length(new.packages)) install.packages(new.packages)
lapply(list.of.packages, require, character.only=TRUE)

#Files
source('D:/PAMStudio/dev/git/rSamara/R/AccessLoader.R')
connectDB('D:/PAMStudio/dev/git/BdD_Sorghum_20151107.accdb')
# parameterFilename <- paste(PPath, "ECOMERISTEM_parameters.txt", sep = "")
ValParamFilename <- paste(PPath, ValParamName, sep = "")
meteo <- loadMeteo('5', '2014/06/01', '2014/11/21')
parameterFileContents <- loadSimDetails('BAMA2014S1', 'CSM63E', 'SotubaPCDA_SV21', '5', '2014/06/01', '2014/11/21')
ValParam <- read.table(ValParamFilename, blank.lines.skip = TRUE, col.names = c("Name", "BInf", "BSup", "Step"))
nbFact <- nrow(ValParam)
ParamAFaireVarier <- as.vector(ValParam[,1])

#Bornes
BornesParamAEstimer <- as.matrix(ValParam[,2:4]) #C1=borne Inf et C2=borne Sup + conversion data.frame en matrice
B1 <- BornesParamAEstimer[,1]
B2 <- BornesParamAEstimer[,2]
Step <- BornesParamAEstimer[,3]
NbParam <- length(as.vector(B1))

bounds <- apply(cbind(B1, B2), 1, function(x){list(min = x[1], max = x[2])})

Samar.fun <- function(x) {
  # recherche de l'indice du parametre en cours dans le fichier de parametre
  # ind <- match(ParamAFaireVarier,parameterFileContents$Name)
  # substitution de la valeur du parametre
  parameterFileContents$ParamAFaireVarier <- x
  write.table(parameterFileContents,
            file = parameterFilename,
            quote = FALSE,
            sep = "=",
            row.names = FALSE,
            col.names = FALSE)
  #On lance la simulation
  SamSim <- rsamara::run2DF(parameterFileContents, meteo)

  #On r?cup?re LA sortie choisie ? l'aide l'interface d'optimisation
  #Attention il faut une seule valeur pas un vecteur
  ValSimTot <- EcoSim[[OVariable]]
  #on r?cup?re la derni?re valeur
  ValSim <- as.numeric(ValSimTot[length(ValSimTot)])
  return(ValSim)
}

Samar <- function(m) {toto=apply(m, 1, Samar); return(toto)}

########################Analyse de Sensibilit?##################################
#M?thode de Morris
SensitivitySamar <- function() {
  sa<-morris(model=NULL,
             factors = ParamAFaireVarier,
             r = 100,
             binf=B1,
             bsup=B2,
             design = list(type = "oat", levels = 10, grid.jump = 5))
  y <- Samar.fun(sa$X)
  tell(sa,y)
  print(sa)
  plot(sa)
}

#PlanComplet
PlanComplet <- function() {
  ListeFacteurs <- vector("list", NbParam)
  for (i in 1:NbParam){
    ListeFacteurs[[i]] <- seq(B1[i], B2[i], Step[i])
  }
  PlanComplet <- expand.grid(ListeFacteurs)
  y <- Samar.fun(PlanComplet)
  sortie <- cbind(PlanComplet, y)
  write.table(sortie,
              paste(PPath, "sortie_plan_complet.txt", sep = ""),
              sep="\t",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
}

##M?thode FAST (A ADAPTER)
IndicesSensitivitySamar <- function() {
  sa <- fast99(model = NULL,
             factors = 4,
             n = 450,
             q = rep("qunif", 4),
             q.arg=bounds)
  y <- Samar.fun(sa$X)

  tell(sa, y)

  sortie <- cbind(sa$X, sa$y)
  write.table(sortie,
              paste(PPath, "sortie_plan_fast.txt", sep = ""),
              sep="\t",
              row.names = FALSE,
              col.names = FALSE,
              dec = ".")
  print(sa)
  plot(sa)
}

########################Start Analyse##################################
switch(Method,
       "M" = {
         SensitivitySamar()
       },
       "C" = {
         IndicesSensitivitySamar()
       },
       "F" = {
         PlanComplet()
       }
)
