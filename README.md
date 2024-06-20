# Samara with R bindings
Samara C++ library with R bindings

## Documentation

The documentation is here :
https://umr-agap.cirad.fr/en/research/scientific-teams/samara-model

## Install from cran

- install.packages("samara")

## Build from project

- install R 
- open Rproj with RStudio 
- build it 

### Installation with conda

Create a conda enironment with R 
- conda create -n samara -c conda-forge "r-base ==3.6.3"
- conda activate samara
- conda install rstudio r-rodbc r-rcpp r-gdata r-stringr -c conda-forge

After cloning the repository
```bash
git clone https://github.com/PAMDeveloper/rSamara
cd rSamara
rstudio rSamara.RProj
```

Then build and install it from the GUI.

## Layout

In the src directory, the model Samara is implemented in C++:
* processes/processes.h : list all the functions
* util/* : list all the utilities (in/out, time, parameters, ...)

* parameters.h : Parameter management of Artis framework
* rccp_samara.cpp : R bindings (convert to/from R/cpp)
* samara.h / samara.cpp : call t the different models SAMARA with different options (e.g. lodging)
* samara_defines.h : misc functions and defines
* samara_parameters.h : init the samara parameters

## Tutorial
https://github.com/GBeurier/Samara_Tutorial
