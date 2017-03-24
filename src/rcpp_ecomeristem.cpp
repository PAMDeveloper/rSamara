/**
 * @file rcpp_samara.cpp
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
#include <Rinternals.h>

#include "recomeristem_types.hpp"

#include <model/kernel/KernelModel.hpp>
#include <model/kernel/Simulator.hpp>
#include <model/models/ModelParameters.hpp>
#include <model/observer/PlantView.hpp>
#include <model/observer/View.hpp>
#include <model/observer/Observer.hpp>
#include <utils/ParametersReader.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>

using namespace Rcpp;

static void format_dates(const model::models::ModelParameters& parameters,
                         std::string& begin, std::string& end)
{
    utils::DateTime::format_date(parameters.get < std::string >("BeginDate"),
                                 begin);
    utils::DateTime::format_date(parameters.get < std::string >("EndDate"),
                                 end);
}

static bool is_number(const std::string& s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();
}

static double cint(double x){
  if (modf(x,0)>=.5)
    return x>=0?ceil(x):floor(x);
  else
    return x<0?ceil(x):floor(x);
}

static double round(double r,unsigned places){
  double off=pow(10,places);
  return cint(r*off)/off;
}

// [[Rcpp::export]]
List getParameters_from_files(Rcpp::String folder)
{
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromFiles(folder, parameters);

    std::map < std::string, std::string > * paramMap = parameters.getRawParameters();
    Rcpp::List result(paramMap->size());
    Rcpp::CharacterVector names;
    Rcpp::CharacterVector values;
    for(auto const& it: *paramMap){
        std::string key = it.first;
        std::string value = it.second;
        if(is_number(value)) {
          value = boost::lexical_cast<std::string>(round(std::stod(value.c_str()),4));
        }
        names.push_back(key);
        values.push_back(value);
    }

    DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
    return df;
}


// [[Rcpp::export]]
List getMeteo_from_files(Rcpp::String folder)
{
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromFiles(folder, parameters);

    std::vector < model::models::Climate > * meteoValues = parameters.getMeteoValues();
    Rcpp::List result(meteoValues->size());
    CharacterVector names =  CharacterVector::create("Temperature", "Par", "Etp", "Irrigation", "P");
    NumericVector Temperature, Par, Etp, Irrigation, P;

    for(auto const& it: *meteoValues){
        Temperature.push_back(it.Temperature);
        Par.push_back(it.Par);
        Etp.push_back(it.Etp);
        Irrigation.push_back(it.Irrigation);
        P.push_back(it.P);
    }

    DataFrame df = DataFrame::create(
                Named("Temperature")=Temperature,
                Named("Par")=Par,
                Named("Etp")=Etp,
                Named("Irrigation")=Irrigation,
                Named("P")=P
            );
    return df;
}

// [[Rcpp::export]]
List getParameters_from_database(Rcpp::String name)
{
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromProstgresql(name, parameters);

    std::map < std::string, std::string > * paramMap = parameters.getRawParameters();
    Rcpp::List result(paramMap->size());
    Rcpp::CharacterVector names;
    Rcpp::CharacterVector values;
    for(auto const& it: *paramMap){
        std::string key = it.first;
        std::string value = it.second;
        names.push_back(key);
        values.push_back(value);
    }

    DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
    return df;
}


// [[Rcpp::export]]
List getMeteo_from_database(Rcpp::String name)
{
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    reader.loadParametersFromProstgresql(name, parameters);

    std::vector < model::models::Climate > * meteoValues = parameters.getMeteoValues();
    Rcpp::List result(meteoValues->size());
    CharacterVector names =  CharacterVector::create("Temperature", "Par", "Etp", "Irrigation", "P");
    NumericVector Temperature, Par, Etp, Irrigation, P;

    for(auto const& it: *meteoValues){
        Temperature.push_back(it.Temperature);
        Par.push_back(it.Par);
        Etp.push_back(it.Etp);
        Irrigation.push_back(it.Irrigation);
        P.push_back(it.P);
    }

    DataFrame df = DataFrame::create(
                Named("Temperature")=Temperature,
                Named("Par")=Par,
                Named("Etp")=Etp,
                Named("Irrigation")=Irrigation,
                Named("P")=P
            );
    return df;
}

// [[Rcpp::export]]
XPtr < Context > rcpp_init_from_database(Rcpp::String name)
{
    Context* context = new Context;
    ecomeristem::GlobalParameters globalParameters;
    model::kernel::KernelModel* model = new model::kernel::KernelModel;
    model::models::ModelParameters parameters;
    utils::ParametersReader reader;
    std::string begin;
    std::string end;

    reader.loadParametersFromProstgresql(name, parameters);
    begin = parameters.get<std::string>("BeginDate");
    end = parameters.get<std::string>("EndDate");
//    globalParameters.modelVersion = parameters.get < std::string >("idmodele");

    context->begin = utils::DateTime::toJulianDayNumber(begin);
    context->end = utils::DateTime::toJulianDayNumber(end);
    context->simulator = new model::kernel::Simulator(model, globalParameters);

    model::observer::PlantView * view = new model::observer::PlantView();
    context->simulator->attachView("global", view);
    context->simulator->init(context->begin, parameters);
    return XPtr < Context >(context, true);
}


//// [[Rcpp::export]]
//XPtr < Context > rcpp_init_from_json(Rcpp::String json)
//{
//    Context* context = new Context;
//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    std::string begin;
//    std::string end;

//    reader.loadFromJSON(json, parameters);
//    format_dates(parameters, begin, end);
//    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

//    context->begin = utils::DateTime::toJulianDayNumber(begin);
//    context->end = utils::DateTime::toJulianDayNumber(end);
//    context->simulator = new model::kernel::Simulator(model, globalParameters);

//    context->simulator->attachView("global", new model::observer::GlobalView);
//    context->simulator->init(context->begin, parameters);
//    return XPtr < Context >(context, true);
//}

//// [[Rcpp::export]]
//XPtr < Context > rcpp_init_from_dataframe(Rcpp::List data)
//{
//    Context* context = new Context;
//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    std::string begin;
//    std::string end;
//    boost::property_tree::ptree tree;
//    CharacterVector names = data.attr("names");

//    for (unsigned int i = 0; i < names.size(); ++i) {
//        std::string key = Rcpp::as < std::string >(names[i]);
//        SEXP s = data[i];

//        if (TYPEOF(s) == 14) {
//            tree.put(key, Rcpp::as < double >(data[i]));
//        } else if (TYPEOF(s) == 16) {
//            tree.put(key, Rcpp::as < std::string >(data[i]));
//        } else if (TYPEOF(s) == 19) { // VECSEXP
//            List subdata = data[i];
//            CharacterVector subnames = subdata.attr("names");
//            boost::property_tree::ptree subtree;

//            for (unsigned int j = 0; j < subnames.size(); ++j) {
//                std::string subkey = Rcpp::as < std::string >(subnames[j]);
//                SEXP t = subdata[j];

//                if (TYPEOF(t) == 14) {
//                    subtree.put(subkey, Rcpp::as < double >(subdata[j]));
//                } else if (TYPEOF(t) == 16) {
//                    subtree.put(subkey, Rcpp::as < std::string >(subdata[j]));
//                }
//            }
//            tree.add_child(key, subtree);
//        }
//    }
//    reader.loadFromTree(tree, parameters);
//    format_dates(parameters, begin, end);
//    globalParameters.modelVersion = parameters.get < std::string >("IdModele");

//    context->begin = utils::DateTime::toJulianDayNumber(begin);
//    context->end = utils::DateTime::toJulianDayNumber(end);
//    context->simulator = new model::kernel::Simulator(model, globalParameters);
//    context->simulator->attachView("global", new model::observer::GlobalView);
//    context->simulator->init(context->begin, parameters);
//    return XPtr < Context >(context, true);
//}

// [[Rcpp::export]]
List rcpp_run(SEXP handle)
{
    XPtr < Context > context(handle);

    context->simulator->run(context->begin, context->end);

    const model::observer::Observer& observer = context->simulator->observer();
    const model::observer::Observer::Views& views = observer.views();

    Rcpp::List gresult(views.size());
    Rcpp::CharacterVector gnames;
    unsigned int gindex = 0;

    for (model::observer::Observer::Views::const_iterator it = views.begin();
         it != views.end(); ++it) {
        Rcpp::List result(it->second->values().size() + 1);
        Rcpp::CharacterVector names;
        model::observer::View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        gnames.push_back(it->first);
        // write header
        names.push_back("time");
        for (model::observer::View::Values::const_iterator
             itv = values.begin(); itv != values.end(); ++itv) {
            names.push_back(itv->first);
        }
        // write dates
        {
            Rcpp::CharacterVector values;

            for (double t = begin; t <= end; ++t) {
                values.push_back(utils::DateTime::toJulianDay(t));
            }
            result[0] = values;
        }
        // write values
        unsigned int index = 1;

        for (model::observer::View::Values::const_iterator itv =
             values.begin(); itv != values.end(); ++itv) {
            model::observer::View::Value::const_iterator itp =
                    itv->second.begin();
            Rcpp::NumericVector values;

            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }
                if (itp != itv->second.end()) {
                    values.push_back(
                                boost::lexical_cast < double >(itp->second));
                } else {
                    values.push_back(NumericVector::get_na());
                }
            }
            result[index] = values;
            ++index;
        }
        DataFrame out(result);

        out.attr("names") = names;
        gresult[gindex] = out;
        ++gindex;
    }

    gresult.attr("names") = gnames;
    return gresult;
}

// [[Rcpp::export]]
List rcpp_run_from_dataframe(List dfParameters, List dfMeteo)
{

    ecomeristem::GlobalParameters globalParameters;
    model::kernel::KernelModel* model = new model::kernel::KernelModel;
    model::models::ModelParameters parameters;

    CharacterVector names = dfParameters[0];
    CharacterVector values = dfParameters[1];
    for (int i = 0; i < names.size(); ++i) {
      //std::cout << names(i) << " " << values(i) << std::endl;
        parameters.getRawParameters()->insert(
            std::pair<std::string,std::string>(
                Rcpp::as<std::string>(names(i)),
                Rcpp::as<std::string>(values(i))
            )
            );
    }

    NumericVector Temperature = dfMeteo[0];
    NumericVector Par = dfMeteo[1];
    NumericVector Etp = dfMeteo[2];
    NumericVector Irrigation = dfMeteo[3];
    NumericVector P = dfMeteo[4];


    for (int i = 0; i < Temperature.size(); ++i) {
      model::models::Climate c(Temperature(i), Par(i), Etp(i), Irrigation(i), P(i));
      parameters.meteoValues.push_back(c);
    }

    std::cout << parameters.get<std::string>("BeginDate") << "\n"
              << parameters.get<std::string>("EndDate") << std::endl;
    double begin = utils::DateTime::toJulianDayNumber(
      parameters.get<std::string>("BeginDate"));
    double end = utils::DateTime::toJulianDayNumber(
      parameters.get<std::string>("EndDate"));
    model::kernel::Simulator * simulator = new model::kernel::Simulator(model, globalParameters);
    simulator->attachView("global", new model::observer::PlantView());
    simulator->init(begin, parameters);
    simulator->run(begin, end);

    const model::observer::Observer& observer = simulator->observer();
    const model::observer::Observer::Views& views = observer.views();
    Rcpp::List gresult(views.size());
    Rcpp::CharacterVector gnames;
    unsigned int gindex = 0;

    for (model::observer::Observer::Views::const_iterator it = views.begin();
         it != views.end(); ++it) {
        Rcpp::List result(it->second->values().size() + 1);
        Rcpp::CharacterVector names;
        model::observer::View::Values values = it->second->values();
        double begin = it->second->begin();
        double end = it->second->end();

        gnames.push_back(it->first);
        // write header
        names.push_back("time");
        for (model::observer::View::Values::const_iterator
             itv = values.begin(); itv != values.end(); ++itv) {
            names.push_back(itv->first);
        }
        // write dates
        {
            Rcpp::CharacterVector values;

            for (double t = begin; t <= end; ++t) {
                values.push_back(utils::DateTime::toJulianDay(t));
            }
            result[0] = values;
        }
        // write values
        unsigned int index = 1;

        for (model::observer::View::Values::const_iterator itv =
             values.begin(); itv != values.end(); ++itv) {
            model::observer::View::Value::const_iterator itp =
                    itv->second.begin();
            Rcpp::NumericVector values;

            for (double t = begin; t <= end; ++t) {
                while (itp != itv->second.end() and itp->first < t) {
                    ++itp;
                }
                if (itp != itv->second.end()) {
                    values.push_back(
                                boost::lexical_cast < double >(itp->second));
                } else {
                    values.push_back(NumericVector::get_na());
                }
            }
            result[index] = values;
            ++index;
        }
        DataFrame out(result);

        out.attr("names") = names;
        gresult[gindex] = out;
        ++gindex;
    }

    gresult.attr("names") = gnames;
    return gresult;
//return 0;
}
