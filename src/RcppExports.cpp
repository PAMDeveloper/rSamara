/**
 * @file RcppExports.cpp
 * @author See the AUTHORS file
 */

/*
 * Copyright (C) 2012-2017 ULCO http://www.univ-littoral.fr
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Rcpp.h>

#include "rcpp_samara.hpp"

using namespace Rcpp;

// rcpp_init_from_database
XPtr < Context > rcpp_init_from_database(Rcpp::String name);
RcppExport SEXP rsamara_rcpp_init_from_database(SEXP s) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_init_from_database(s));
    return rcpp_result_gen;
END_RCPP
}

// rcpp_init_from_json
XPtr < Context > rcpp_init_from_json(Rcpp::String name);
RcppExport SEXP rsamara_rcpp_init_from_json(SEXP s) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_init_from_json(s));
    return rcpp_result_gen;
END_RCPP
}

// rcpp_init_from_dataframe
XPtr < Context > rcpp_init_from_dataframe(Rcpp::List data);
RcppExport SEXP rsamara_rcpp_init_from_dataframe(SEXP s) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_init_from_dataframe(s));
    return rcpp_result_gen;
END_RCPP
}

// rcpp_run
List rcpp_run(SEXP handle);
RcppExport SEXP rsamara_rcpp_run(SEXP s) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_run(s));
    return rcpp_result_gen;
END_RCPP
}
