x <<- list()
x[[1]] <- 3

a_func <- function() {
  b_func <- function() {
    print(paste("inside before:", x))

    x <- get("x", parent.frame())[[1]]
    x[[1]] <- 4
    assign("x", x, parent.frame())
    print(paste("inside after:", x))
  }
  return(b_func)
}

print(paste("before:", x))
a_func()()
print(paste("after:", x))


t <- paste(rev(strsplit("08/12/2013", "/")[[1]]), collapse = "-")
