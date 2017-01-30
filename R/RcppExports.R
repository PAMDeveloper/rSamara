 ##
 # @file RcppExports.cpp
 # @author See the AUTHORS file
 ##

 ##
 # Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 #
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ##

rcpp_init_from_database <- function(name) {
    .Call('rsamara_rcpp_init_from_database', PACKAGE = 'rsamara', name)
}

rcpp_init_from_json <- function(json) {
    .Call('rsamara_rcpp_init_from_json', PACKAGE = 'rsamara', json)
}

rcpp_init_from_dataframe <- function(data) {
    .Call('rsamara_rcpp_init_from_dataframe', PACKAGE = 'rsamara', data)
}

rcpp_run <- function(context) {
    .Call('rsamara_rcpp_run', PACKAGE = 'rsamara', context)
}
