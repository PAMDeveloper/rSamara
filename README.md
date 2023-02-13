# Samara with R bindings
Samara C++ library with R bindings

## Documentation

The documentation is here :
https://umr-agap.cirad.fr/en/research/scientific-teams/samara-model

## Install

- install R and rtools
- open Rproj
- build it 

## Layout

In the src directory:
* processes/processes.h : list all the functions
* util/* : list all the utilities (in/out, time, parameters, ...)

* parameters.h : Parameter management of Artis framework
* rccp_samara.cpp : R bindings (convert to/from R/cpp)
* samara.h / samara.cpp : call t the different models SAMARA with different options (e.g. lodging)
* samara_defines.h : misc functions and defines
* samara_parameters.h : init the samara parameters


