library(rsamara)
filename <- './example.json'
json <- readChar(filename, file.info(filename)$size)
context = rsamara::rcpp_init_from_json(json)
result = rsamara::rcpp_run(context)
print(result[["global"]]["SommeDegresJour"])