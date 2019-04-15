#Script estimating parameters for Samara model
#Authors : Gregory Beurier -- (PAM, AGAP, BIOS, CIRAD)
#2019

getOS <- function(){
  sysinf <- Sys.info()
  if (!is.null(sysinf)){
    os <- sysinf['sysname']
    if (os == 'Darwin')
      os <- "osx"
  } else { ## mystery machine
    os <- .Platform$OS.type
    if (grepl("^darwin", R.version$os))
      os <- "osx"
    if (grepl("linux-gnu", R.version$os))
      os <- "linux"
  }
  tolower(os)
}

getFilePath <- function (filename, folder, default_filename = "") {
  if(filename != "") {
    filepath <- ifelse( folder == ""
                        , filename
                        , file.path(folder, filename)
    )

    if(file.exists(filepath))
      return (filepath)
  }

  if(default_filename == "")
    return ("")

  filepath <- ifelse( folder == ""
                      , default_filename
                      , file.path(folder, default_filename)
  )

  if(!file.exists(filepath))
    return ("")

  return (filepath)
}

getDirPath <- function (path, parent, default_path = "results", autocreate = TRUE) {
  if(path == "") {
    if(default_path == "")
      return ("")
    else
      path = default_path
  }

  path <- file.path(parent, path)

  if(!dir.exists(path) && !autocreate)
    return ("")

  dir.create(path, recursive = TRUE, showWarnings = FALSE)

  return (path)
}
