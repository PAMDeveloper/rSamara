
resPlot <- function() {
  bestp <- as.vector(res$par)
  paramInitTrans[ParamOfInterest] <- bestp
  parameters <- paramInitTrans #data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]), row.names=NULL)
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res = rsamara::rcpp_reduceResults(Res_samara, vObs)
  obsRed$day <- vObs$day

  plotF <- function(x) {
    plot(Res_samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(min(min(Res_samara[[x]], na.rm=T),min(obsRed[[x]], na.rm=T)),max(max(Res_samara[[x]], na.rm=T),max(obsRed[[x]], na.rm=T))))
    points(obsRed$day, obsRed[[x]], type="p", col="red")
    # if(!is.null(obsETRed[[x]])) {
    #   arrows(obsRed$day,obsRed[[x]]-obsETRed[[x]],obsRed$day,obsRed[[x]]+obsETRed[[x]], code=3, length=0.02, angle = 90)
    # }
    if(!is.null(obsRed[[x]])) {
      return(paste((abs(sum((abs((obsRed[[x]] - res[[x]])))/(obsRed[[x]]), na.rm=T))*100),"%", sep=""))
    } else {
      return(NULL)
    }
  }
  sapply(VarList, plotF)
}

dePlot <- function() {
  par(mfrow=c(1,1))
  plot(resOptim, type="b")
  par(mfrow=c(1,1))
  plot(resOptim, plot.type="bestvalit",type="l")
}

allPlot <- function() {
  bestp <- as.vector(res$par)
  paramInitTrans[ParamOfInterest] <- bestp
  parameters <- paramInitTrans#data.frame(Name=ParamList, Values=unlist(paramInitTrans[1,]), row.names=NULL)
  Res_samara <- rsamara::run2DF(parameters,meteo)
  res = rsamara::rcpp_reduceResults(Res_samara, vObs)
  obsRed$day <- vObs$day

  plotF <- function(x) {
    plot(Res_samara[[x]], type="l", xlab="DAS", ylab=x,ylim=c(1.5*min(Res_samara[[x]], na.rm=T),1.5*max(Res_samara[[x]], na.rm=T)))
    return("Done")
  }
  sapply(names(Res_samara), plotF)
}
