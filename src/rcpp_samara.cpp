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
#include "rsamara_types.hpp"

using namespace Rcpp;
using namespace std;


List DFFromDoubleMap(const map <string, pair <double, string> > & map, string category)
{
    Rcpp::CharacterVector names;
    Rcpp::NumericVector values;
    for(auto const& it: map){
        string key = it.first;
        double value = it.second.first;
        string cat = it.second.second;
        if(cat == category || category == "") {
            names.push_back(key);
            values.push_back(value);
        }
    }
    DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
    return df;
}

List DFFromStringMap(const map <string, pair <string, string> > & map, string category)
{
    Rcpp::CharacterVector names;
    Rcpp::CharacterVector values;
    for(auto const& it: map){
        string key = it.first;
        string value = it.second.first;
        string cat = it.second.second;
        if(cat == category || category == "") {
            names.push_back(key);
            values.push_back(value);
        }
    }
    DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
    return df;
}

List DFFromClimaticVector(vector < Climate > meteoValues)
{
    NumericVector TMax, TMin, TMoy, HMax, HMin, HMoy, Vt, Ins, Rg, ETP, Rain;
    for(auto const& it: meteoValues){
        TMax.push_back(it.TMax);
        TMin.push_back(it.TMin);
        TMoy.push_back(it.TMoy);
        HMax.push_back(it.HMax);
        HMin.push_back(it.HMin);
        HMoy.push_back(it.HMoy);
        Vt.push_back(it.Vt);
        Ins.push_back(it.Ins);
        Rg.push_back(it.Rg);
        ETP.push_back(it.ETP);
        Rain.push_back(it.Rain);
    }

    DataFrame df = DataFrame::create(
                Named("TMax")=TMax,
                Named("TMin")=TMin,
                Named("TMoy")=TMoy,
                Named("HMax")=HMax,
                Named("HMin")=HMin,
                Named("HMoy")=HMoy,
                Named("Vt")=Vt,
                Named("Ins")=Ins,
                Named("Rg")=Rg,
                Named("ETP")=ETP,
                Named("Rain")=Rain
            );
    return df;
}

List DFFromDoubleRecord(string table, string key, string value) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_record(table, key, value);
    List result = DFFromDoubleMap(parameters->doubles, table);
    delete parameters;
    delete loader;
    return result;
}

List DFFromStringRecord(string table, string key, string value) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_record(table, key, value);
    List result = DFFromStringMap(parameters->strings, table);
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List DBSimulationParamsDF(Rcpp::String idsimulation) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_complete_simulation(idsimulation);
    List result = DFFromDoubleMap(parameters->doubles, "");
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List DBSimulationMeteoDF(Rcpp::String idsimulation) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_complete_simulation(idsimulation);
    List result = DFFromClimaticVector(parameters->climatics);
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List DBSimulationIdDF(Rcpp::String idsimulation) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_complete_simulation(idsimulation);
    List result = DFFromStringMap(parameters->strings, "");
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List DBSimpleSimulationParamsDF(Rcpp::String idsimulation) {
    return DFFromDoubleRecord("simulation","idsimulation",idsimulation);
}

// [[Rcpp::export]]
List DBVarietyDF(Rcpp::String idvariete) {
    return DFFromDoubleRecord("variete","idvariete",idvariete);
}

// [[Rcpp::export]]
List DBStationDF(Rcpp::String codestation) {
    return DFFromDoubleRecord("station","codestation",codestation);
}

// [[Rcpp::export]]
List DBPlotDF(Rcpp::String idparcelle) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_plot(idparcelle);
    List result = DFFromDoubleMap(parameters->doubles, "parcelle");
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List DBItineraryDF(Rcpp::String iditinerairetechnique) {
    return DFFromDoubleRecord("itinerairetechnique","iditinerairetechnique",iditinerairetechnique);
}

// [[Rcpp::export]]
List DBMeteoDF(Rcpp::String codestation, Rcpp::String beginDate, Rcpp::String endDate) {
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_meteo(codestation, beginDate, endDate);
    List result = DFFromClimaticVector(parameters->climatics);
    delete parameters;
    delete loader;
    return result;
}


//List getFileParameters(Rcpp::String filepath, Rcpp::String id){}

void fillMapWithDoubleList(map <string, pair <double, string> > & map, List list, string category) {
    CharacterVector names = list[0];
    NumericVector values = list[1];
    for (int i = 0; i < names.size(); ++i) {
        pair <double, string> token( values(i), category );
        map.insert( pair<string, pair <double, string> >(
                        Rcpp::as<string>(names(i)), token )
                    );
        //std::cout << Rcpp::as<string>(names(i)) << values(i) << std::endl;
    }
}

void fillClimaticVectorWithList(vector < Climate > & climatics, List list) {
    NumericVector TMax = list[0];
    NumericVector TMin = list[1];
    NumericVector TMoy = list[2];
    NumericVector HMax = list[3];
    NumericVector HMin = list[4];
    NumericVector HMoy = list[5];
    NumericVector Vt = list[6];
    NumericVector Ins = list[7];
    NumericVector Rg = list[8];
    NumericVector Rain = list[9];
    NumericVector ETP = list[10];
    for (int i = 0; i < TMax.size(); ++i) {
        Climate c(TMax(i), TMin(i), TMoy(i), HMax(i), HMin(i), HMoy(i), Vt(i), Ins(i), Rg(i), Rain(i), ETP(i));
        climatics.push_back(c);
    }
}

DataFrame resultToList(const pair <vector <string>, vector < vector <double> > > & results) {
    CharacterVector names( results.first.begin(), results.first.end() );
    List values(results.first.size());
    for (int i = 0; i < results.first.size(); ++i) {
        NumericVector vValues( results.second[i].begin(), results.second[i].end() );
        values[i] = vValues;
    }
    DataFrame out(values);
    out.attr("names") = names;
    return out;
}


#include <chrono>
#include <ctime>

// [[Rcpp::export]]
List runDB(Rcpp::String idsimulation)
{
    auto startC = std::chrono::high_resolution_clock::now();
    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader * loader = new PSQLLoader(parameters);
    loader->load_complete_simulation(idsimulation);
    auto endC = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli>  elapsed_seconds = endC-startC;
    std::cout << "parameters loaded in " << elapsed_seconds.count() << "ms\n";
    startC = endC;
    init_parameters(parameters);
    List result = resultToList(run_samara_2_1(parameters));
    endC = std::chrono::high_resolution_clock::now();
    elapsed_seconds = endC-startC;
    std::cout << "simulation done in " << elapsed_seconds.count() << "ms\n";
    delete parameters;
    delete loader;
    return result;
}

// [[Rcpp::export]]
List run2DF(List params, List meteo)
{
    SamaraParameters * parameters = new SamaraParameters();
    fillMapWithDoubleList(parameters->doubles, params, "");
    //for (auto const& x : parameters->doubles)
    //{std::cout << x.first << " " << x.second.first << std::endl;}
    fillClimaticVectorWithList(parameters->climatics, meteo);
    //for (auto const& x : parameters->climatics)
    //{std::cout << x.TMax << std::endl;}
    init_parameters(parameters);
    List result = resultToList(run_samara_2_1(parameters));
    delete parameters;
    return result;
}

// [[Rcpp::export]]
List runDF(Rcpp::String from_date, Rcpp::String to_date, List simulation, List variety, List plot, List itinerary, List station, List meteo)
{
    SamaraParameters * parameters = new SamaraParameters();
    fillMapWithDoubleList(parameters->doubles, simulation, "simulation");
    fillMapWithDoubleList(parameters->doubles, variety, "variete");
    fillMapWithDoubleList(parameters->doubles, plot, "parcelle");
    fillMapWithDoubleList(parameters->doubles, itinerary, "itinerairetechnique");
    fillMapWithDoubleList(parameters->doubles, station, "station");

    fillClimaticVectorWithList(parameters->climatics, meteo);

    parameters->doubles.insert( pair<string ,pair <double, string> >( "datedebut",
                                                                      pair <double, string>(JulianDayConverter::toJulianDayNumber(from_date), "simulation") ));
    parameters->doubles.insert( pair<string ,pair <double, string> >( "datefin",
                                                                      pair <double, string>(JulianDayConverter::toJulianDayNumber(to_date), "simulation") ));

    init_parameters(parameters);
    List result = resultToList(run_samara_2_1(parameters));
    delete parameters;
    return result;
}

















//// [[Rcpp::export]]
//XPtr < Context > rcpp_init_from_database(Rcpp::String name)
//{
//    Context* context = new Context;
//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;
//    utils::ParametersReader reader;
//    string begin;
//    string end;

//    reader.loadFromDatabase(name, parameters);
//    begin = parameters.get<string>("datedebut");
//    end = parameters.get<string>("datefin");
//    globalParameters.modelVersion = parameters.get < string >("idmodele");

//    context->begin = utils::DateTime::toJulianDayNumber(begin);
//    context->end = utils::DateTime::toJulianDayNumber(end);
//    context->simulator = new model::kernel::Simulator(model, globalParameters);

//    context->simulator->attachView("global", new model::observer::GlobalView());
//    context->simulator->init(context->begin, parameters);
//    return XPtr < Context >(context, true);
//}


////// [[Rcpp::export]]
////XPtr < Context > rcpp_init_from_json(Rcpp::String json)
////{
////    Context* context = new Context;
////    samara::GlobalParameters globalParameters;
////    model::kernel::KernelModel* model = new model::kernel::KernelModel;
////    model::models::ModelParameters parameters;
////    utils::ParametersReader reader;
////    string begin;
////    string end;

////    reader.loadFromJSON(json, parameters);
////    format_dates(parameters, begin, end);
////    globalParameters.modelVersion = parameters.get < string >("IdModele");

////    context->begin = utils::DateTime::toJulianDayNumber(begin);
////    context->end = utils::DateTime::toJulianDayNumber(end);
////    context->simulator = new model::kernel::Simulator(model, globalParameters);

////    context->simulator->attachView("global", new model::observer::GlobalView);
////    context->simulator->init(context->begin, parameters);
////    return XPtr < Context >(context, true);
////}

////// [[Rcpp::export]]
////XPtr < Context > rcpp_init_from_dataframe(Rcpp::List data)
////{
////    Context* context = new Context;
////    samara::GlobalParameters globalParameters;
////    model::kernel::KernelModel* model = new model::kernel::KernelModel;
////    model::models::ModelParameters parameters;
////    utils::ParametersReader reader;
////    string begin;
////    string end;
////    boost::property_tree::ptree tree;
////    CharacterVector names = data.attr("names");

////    for (unsigned int i = 0; i < names.size(); ++i) {
////        string key = Rcpp::as < string >(names[i]);
////        SEXP s = data[i];

////        if (TYPEOF(s) == 14) {
////            tree.put(key, Rcpp::as < double >(data[i]));
////        } else if (TYPEOF(s) == 16) {
////            tree.put(key, Rcpp::as < string >(data[i]));
////        } else if (TYPEOF(s) == 19) { // VECSEXP
////            List subdata = data[i];
////            CharacterVector subnames = subdata.attr("names");
////            boost::property_tree::ptree subtree;

////            for (unsigned int j = 0; j < subnames.size(); ++j) {
////                string subkey = Rcpp::as < string >(subnames[j]);
////                SEXP t = subdata[j];

////                if (TYPEOF(t) == 14) {
////                    subtree.put(subkey, Rcpp::as < double >(subdata[j]));
////                } else if (TYPEOF(t) == 16) {
////                    subtree.put(subkey, Rcpp::as < string >(subdata[j]));
////                }
////            }
////            tree.add_child(key, subtree);
////        }
////    }
////    reader.loadFromTree(tree, parameters);
////    format_dates(parameters, begin, end);
////    globalParameters.modelVersion = parameters.get < string >("IdModele");

////    context->begin = utils::DateTime::toJulianDayNumber(begin);
////    context->end = utils::DateTime::toJulianDayNumber(end);
////    context->simulator = new model::kernel::Simulator(model, globalParameters);
////    context->simulator->attachView("global", new model::observer::GlobalView);
////    context->simulator->init(context->begin, parameters);
////    return XPtr < Context >(context, true);
////}

//// [[Rcpp::export]]
//List rcpp_run(SEXP handle)
//{
//    XPtr < Context > context(handle);

//    context->simulator->run(context->begin, context->end);

//    const model::observer::Observer& observer = context->simulator->observer();
//    const model::observer::Observer::Views& views = observer.views();
//    Rcpp::List gresult(views.size());
//    Rcpp::CharacterVector gnames;
//    unsigned int gindex = 0;

//    for (model::observer::Observer::Views::const_iterator it = views.begin();
//         it != views.end(); ++it) {
//        Rcpp::List result(it->second->values().size() + 1);
//        Rcpp::CharacterVector names;
//        model::observer::View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        gnames.push_back(it->first);
//        // write header
//        names.push_back("time");
//        for (model::observer::View::Values::const_iterator
//             itv = values.begin(); itv != values.end(); ++itv) {
//            names.push_back(itv->first);
//        }
//        // write dates
//        {
//            Rcpp::CharacterVector values;

//            for (double t = begin; t <= end; ++t) {
//                values.push_back(utils::DateTime::toJulianDay(t));
//            }
//            result[0] = values;
//        }
//        // write values
//        unsigned int index = 1;

//        for (model::observer::View::Values::const_iterator itv =
//             values.begin(); itv != values.end(); ++itv) {
//            model::observer::View::Value::const_iterator itp =
//                    itv->second.begin();
//            Rcpp::NumericVector values;

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }
//                if (itp != itv->second.end()) {
//                    values.push_back(
//                                boost::lexical_cast < double >(itp->second));
//                } else {
//                    values.push_back(NumericVector::get_na());
//                }
//            }
//            result[index] = values;
//            ++index;
//        }
//        DataFrame out(result);

//        out.attr("names") = names;
//        gresult[gindex] = out;
//        ++gindex;
//    }

//    gresult.attr("names") = gnames;
//    return gresult;
//}

//// [[Rcpp::export]]
//List rcpp_run_from_dataframe(List dfParameters, List dfMeteo)
//{

//    samara::GlobalParameters globalParameters;
//    model::kernel::KernelModel* model = new model::kernel::KernelModel;
//    model::models::ModelParameters parameters;

//    CharacterVector names = dfParameters[0];
//    CharacterVector values = dfParameters[1];
//    for (int i = 0; i < names.size(); ++i) {
//      //cout << names(i) << " " << values(i) << endl;
//        parameters.getRawParameters()->insert(
//            pair<string,string>(
//                Rcpp::as<string>(names(i)),
//                Rcpp::as<string>(values(i))
//            )
//            );
//    }

//    NumericVector TMax = dfMeteo[0];
//    NumericVector TMin = dfMeteo[1];
//    NumericVector TMoy = dfMeteo[2];
//    NumericVector HMax = dfMeteo[3];
//    NumericVector HMin = dfMeteo[4];
//    NumericVector HMoy = dfMeteo[5];
//    NumericVector Vt = dfMeteo[6];
//    NumericVector Ins = dfMeteo[7];
//    NumericVector Rg = dfMeteo[8];
//    NumericVector ETP = dfMeteo[9];
//    NumericVector Rain = dfMeteo[10];

//    for (int i = 0; i < TMax.size(); ++i) {
//      model::models::Climate c(TMax(i), TMin(i), TMoy(i), HMax(i), HMin(i), HMoy(i), Vt(i), Ins(i), Rg(i), Rain(i));
//      parameters.meteoValues.push_back(c);
//    }

//    /*cout << parameters.get<string>("datedebut") << "\n"
//                << parameters.get<string>("datefin") << endl;*/
//    double begin = utils::DateTime::toJulianDayNumber(
//      parameters.get<string>("datedebut"));
//    double end = utils::DateTime::toJulianDayNumber(
//      parameters.get<string>("datefin"));
//    model::kernel::Simulator * simulator = new model::kernel::Simulator(model, globalParameters);
//    simulator->attachView("global", new model::observer::GlobalView());
//    simulator->init(begin, parameters);
//    simulator->run(begin, end);


//    for(auto it = parameters.getRawParameters()->begin(); it != parameters.getRawParameters()->end(); ++it) {
//      cout << it->first << " " << it->second << "\n";
//    }

//    const model::observer::Observer& observer = simulator->observer();
//    const model::observer::Observer::Views& views = observer.views();
//    Rcpp::List gresult(views.size());
//    Rcpp::CharacterVector gnames;
//    unsigned int gindex = 0;

//    for (model::observer::Observer::Views::const_iterator it = views.begin();
//         it != views.end(); ++it) {
//        Rcpp::List result(it->second->values().size() + 1);
//        Rcpp::CharacterVector names;
//        model::observer::View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        gnames.push_back(it->first);
//        // write header
//        names.push_back("time");
//        for (model::observer::View::Values::const_iterator
//             itv = values.begin(); itv != values.end(); ++itv) {
//            names.push_back(itv->first);
//        }
//        // write dates
//        {
//            Rcpp::CharacterVector values;

//            for (double t = begin; t <= end; ++t) {
//                values.push_back(utils::DateTime::toJulianDay(t));
//            }
//            result[0] = values;
//        }
//        // write values
//        unsigned int index = 1;

//        for (model::observer::View::Values::const_iterator itv =
//             values.begin(); itv != values.end(); ++itv) {
//            model::observer::View::Value::const_iterator itp =
//                    itv->second.begin();
//            Rcpp::NumericVector values;

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }
//                if (itp != itv->second.end()) {
//                    values.push_back(
//                                boost::lexical_cast < double >(itp->second));
//                } else {
//                    values.push_back(NumericVector::get_na());
//                }
//            }
//            result[index] = values;
//            ++index;
//        }
//        DataFrame out(result);

//        out.attr("names") = names;
//        gresult[gindex] = out;
//        ++gindex;
//    }

//    gresult.attr("names") = gnames;
//    return gresult;
////return 0;
//}
