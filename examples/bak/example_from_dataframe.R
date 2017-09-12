library(jsonlite)
library(rsamara)
filename <- './example.json'
data <- fromJSON(filename)
context = rsamara::rcpp_init_from_dataframe(data)
result = rsamara::rcpp_run(context)
print(result[["global"]]["SommeDegresJour"])