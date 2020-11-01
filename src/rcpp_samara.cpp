#include <Rcpp.h>
#include <Rinternals.h>
#include "rsamara_types.hpp"

#include <iostream>
#include <fstream>

using namespace Rcpp;
using namespace std;


DataFrame mapOfVectorToDF(std::map<std::string, std::vector<double>> map) {
  Rcpp::CharacterVector names;
  List values(map.size());
  int idx = 0;
  for(auto const& it: map){
    names.push_back(it.first);
    NumericVector vec( it.second.begin(), it.second.end() );
    values[idx] = vec;
    idx++;
  }
  DataFrame df(values);
  df.attr("names") = names;
  return df;
}

map <string, vector <double > > mapFromDF(DataFrame list) {
  map <string, vector <double > > map;
  CharacterVector names = list.attr("names");
  for (int i = 0; i < names.size(); ++i) {
    std::vector<double> vec = as<std::vector<double> >(list[i]);
    string s = Rcpp::as<string>(names(i));
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    std::pair <std::string, std::vector<double> > token( s, vec );
    map.insert( token );
  }
  return map;
}

void fillMapWithDoubleList(map <string, pair <double, string> > & map, List list, string category) {
    CharacterVector names = list.attr("names");
    NumericVector values = list[0];
    for (int i = 0; i < names.size(); ++i) {
        double val = *REAL(list[i]);
        string key = Rcpp::as<string>(names[i]);
        //std::cout << key << std::endl << ":" << val << std::endl;
        pair <double, string> token( val, category );
        map.insert( pair<string, pair <double, string> >(
                        key, token )
        );

    }
}

void fillClimaticVectorWithList(vector < Climate > & climatics, List list) {
    NumericVector TMax = list[3];
    NumericVector TMin = list[2];
    NumericVector TMoy = list[4];
    NumericVector HMax = list[6];
    NumericVector HMin = list[5];
    NumericVector HMoy = list[7];
    NumericVector Vt = list[9];
    NumericVector Ins = list[11];
    NumericVector Rg = list[10];
    NumericVector Rain = list[8];
    NumericVector ETP = list[12];
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
        Rain.push_back(it.Rain);
        ETP.push_back(it.ETP);
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
                Named("Rain")=Rain,
                Named("ETP")=ETP
            );
    return df;
}


/********************************************************************/

SamaraParameters * current_params = nullptr;
std::vector<SamaraParameters*> params_vector;

SamaraParameters * params_sim(List params, List meteo, List str_params) {
  SamaraParameters * sparams = new SamaraParameters();

  CharacterVector names = params.attr("names");
  for (int i = 0; i < params.size(); ++i) {
    CharacterVector column = params[i];
    string key = Rcpp::as<string>(names[i]);
    if(column[0] != "") {
      double val = ::atof(column[0]);
      pair <double, string> token( val, "unestimated" );
      sparams->doubles.insert( pair<string, pair <double, string> >(key, token ));
    }
  }

  CharacterVector str_names = str_params.attr("names");
  for (int i = 0; i < str_params.size(); ++i) {
    CharacterVector column = str_params[i];
    string key = Rcpp::as<string>(str_names[i]);
    if(column[0] != "") {
      string val = Rcpp::as<string>(column[0]);
      pair <string, string> token( val, "unestimated" );
      sparams->strings.insert( pair<string, pair <string, string> >(key, token ));
    }
  }

  fillClimaticVectorWithList(sparams->climatics, meteo);

  return sparams;
}

// [[Rcpp::export]]
void clean() {
  if(current_params != nullptr)
    delete current_params;
  params_vector.clear();
  // if(params_vector.size() > 0) {
  //   for(int i; i< param_vectors.size();i++){
  //     if(param_vectors[i] != nullptr)
  //       delete param_vectors[i];
  //   }
  // }
}

// [[Rcpp::export]]
void init_sim(List params, List meteo, List str_params) {
  if(current_params != nullptr)
    delete current_params;

  current_params = params_sim(params, meteo, str_params);
}

SamaraParameters * params_sim_simple(List params, List meteo) {
  SamaraParameters * sparams = new SamaraParameters();

  CharacterVector names = params.attr("names");
  for (int i = 0; i < params.size(); ++i) {
    CharacterVector column = params[i];
    string key = Rcpp::as<string>(names[i]);
    if(column[0] != "") {
      double val = ::atof(column[0]);
      pair <double, string> token( val, "unestimated" );
      sparams->doubles.insert( pair<string, pair <double, string> >(key, token ));
    }
  }

  fillClimaticVectorWithList(sparams->climatics, meteo);

  return sparams;
}

// [[Rcpp::export]]
void init_sim_idx_simple(int idx, List params, List meteo) {
  std::cout << "init simple sim "<< idx << std::endl;
  while(params_vector.size() < idx)
    params_vector.push_back(nullptr);

  if(params_vector[idx-1] != nullptr) {
    delete params_vector[idx-1];
  }

  params_vector[idx-1] = params_sim_simple(params, meteo);
}

// [[Rcpp::export]]
void init_sim_idx(int idx, List params, List meteo, List str_params) {
  while(params_vector.size() < idx)
    params_vector.push_back(nullptr);

  if(params_vector[idx-1] != nullptr)
    delete params_vector[idx-1];

  params_vector[idx-1] = params_sim(params, meteo, str_params);
}

void update_params(SamaraParameters * params, NumericVector values, CharacterVector names) {
  for (int i = 0; i < names.size(); ++i) {
    double val = values[i];
    string key = Rcpp::as<string>(names[i]);
    pair <double, string> token( val, "estimation" );
    if(params->doubles.find(key) != params->doubles.end()) {
      params->doubles[key] = token;
    } else {
      params->doubles.insert( pair<string, pair <double, string> >(key, token) );
    }
  }
}

// [[Rcpp::export]]
void update_sim(NumericVector values, CharacterVector names) {
  update_params(current_params, values, names);
}

// [[Rcpp::export]]
void update_sim_idx(int idx, NumericVector values, CharacterVector names) {
  update_params(params_vector[idx-1], values, names);
}

List run_params(SamaraParameters * params) {
  Samara samara;
  auto results = samara.run_samara_2_1(params);
  List result = resultToList(results);
  return result;
}

// [[Rcpp::export]]
List run_sim() {
  return run_params(current_params);
}

// [[Rcpp::export]]
List run_sim_idx(int idx) {
  return run_params(params_vector[idx-1]);
}

// [[Rcpp::export]]
List reduce_sim(List results, List vobs) {
  std::map <std::string, std::vector<double> > vObsMap;
  std::map <std::string, std::vector<double> > resultMap;
  vObsMap = mapFromDF(vobs);
  resultMap = mapFromDF(results);

  ResultParser parser;
  auto ret = parser.spl_ReduceResults(resultMap,vObsMap,"nbjas");
  return mapOfVectorToDF(ret);
}

void save_params(SamaraParameters * params, string path) {
  ofstream file(path + ".csv");
  if (file.is_open())
  {
    file << params->strings.size() << "\n";
    // std::cout << current_params->strings.size() << "\n";
    for (auto const& token : params->strings) {
      file << token.first << "\t" << token.second.first << "\n";
      // std::cout << token.first << "\t" << token.second.first << "\n";
    }

    file << params->doubles.size() << "\n";
    // std::cout << current_params->doubles.size() << "\n";
    for (auto const& token : params->doubles) {
      file << fixed << token.first << "\t" << token.second.first << "\n";
      // std::cout << fixed << token.first << "\t" << token.second.first << "\n";
    }

    // file.flush();
    file.close();
  } else {
    std::cout << "Unable to open file " << path << std::endl;
  }

  if ( ! file ) {
    std::cout << "Problem with file " << path << std::endl;
  }
}

// [[Rcpp::export]]
void save_sim(string path) {
  save_params(current_params, path);
}

// [[Rcpp::export]]
void save_sim_idx(int idx, string path) {
  save_params(params_vector[idx-1], path);
}


void print_sim_str_params(SamaraParameters * params) {
  for(auto const & t: params->strings) {
    std::cout << fixed << t.first << ": " << t.second.first << std::endl;
  }
}

void print_sim_dbl_params(SamaraParameters * params) {
  for(auto const & t: params->doubles) {
    std::cout << fixed << t.first << ": " << t.second.first << std::endl;
  }
}

void print_sim_params(SamaraParameters * params) {
  print_sim_str_params(params);
  print_sim_dbl_params(params);
}

// [[Rcpp::export]]
void print_sim_str() {
  print_sim_str_params(current_params);
}
// [[Rcpp::export]]
void print_sim_dbl() {
  print_sim_dbl_params(current_params);
}
// [[Rcpp::export]]
void print_sim() {
  print_sim_params(current_params);
}

// [[Rcpp::export]]
void print_sim_str_idx(int idx) {
  print_sim_str_params(params_vector[idx-1]);
}
// [[Rcpp::export]]
void print_sim_dbl_idx(int idx) {
  print_sim_dbl_params(params_vector[idx-1]);
}
// [[Rcpp::export]]
void print_sim_idx(int idx) {
  print_sim_params(params_vector[idx-1]);
}


// [[Rcpp::export]]
DataFrame params_str_df() {
  return DFFromStringMap(current_params->strings, "");
}
// [[Rcpp::export]]
DataFrame params_str_df_idx(int idx) {
  return DFFromStringMap(params_vector[idx-1]->strings, "");
}

// [[Rcpp::export]]
DataFrame params_dbl_df() {
  return DFFromDoubleMap(current_params->doubles, "");
}
// [[Rcpp::export]]
DataFrame params_dbl_df_idx(int idx) {
  return DFFromDoubleMap(params_vector[idx-1]->doubles, "");
}


// [[Rcpp::export]]
int sim_exist() {
  return current_params != nullptr ? 1 : 0;
}
// [[Rcpp::export]]
int sim_exist_idx(int idx) {
  return (params_vector.size() >= idx && params_vector[idx-1] != nullptr) ? 1 : 0;
}

/********************************************************************/


// [[Rcpp::export]]
List rcpp_reduceResults(List results, List vobs) {
  std::map <std::string, std::vector<double> > vObsMap;
  std::map <std::string, std::vector<double> > resultMap;
  vObsMap = mapFromDF(vobs);
  resultMap = mapFromDF(results);
  ResultParser parser;
  auto ret = parser.reduceResults(resultMap,vObsMap,map<string, double>(),"nbjas");
  return mapOfVectorToDF(ret);
}

// [[Rcpp::export]]
double toJulianDayCalcC(Rcpp::String date, Rcpp::String format, Rcpp::String sep) {
  return JulianCalculator::toJulianDay(date, format, sep);
}

// [[Rcpp::export]]
double toJulianDayCalc(Rcpp::String date) {
  return JulianCalculator::toJulianDay(date, JulianCalculator::YMD, '-');
}

// [[Rcpp::export]]
string toAccessFormat(Rcpp::String date, Rcpp::String format, Rcpp::String sep) {
  double jDay = JulianCalculator::toJulianDay(date, format, sep);
  return JulianCalculator::toStringDate(jDay, JulianCalculator::YMD, '/');
}

// [[Rcpp::export]]
List run2DF(List params, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, params, "");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  Samara samara;
  pair <vector <string>, vector < vector <double> > > results = samara.run_samara_2_1(parameters);
  List result = resultToList(results);
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
  parameters->doubles.insert( pair<string ,pair <double, string> >( "startingdate",
                                                                    pair <double, string>(toJulianDayCalc(from_date), "simulation") ));
  parameters->doubles.insert( pair<string ,pair <double, string> >( "endingdate",
                                                                    pair <double, string>(toJulianDayCalc(to_date), "simulation") ));
  Samara samara;
  List result = resultToList(samara.run_samara_2_1(parameters));
  delete parameters;
  return result;
}

// [[Rcpp::export]]
List runSimpleSamara2_1(List params, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, params, "");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  Samara samara;
  pair <vector <string>, vector < vector <double> > > results = samara.run_samara_2_1(parameters);
  List result = resultToList(results);
  delete parameters;
  return result;
}

// [[Rcpp::export]]
List runSamara2_1(Rcpp::String from_date, Rcpp::String to_date, List simulation, List variety, List plot, List itinerary, List station, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, simulation, "simulation");
  fillMapWithDoubleList(parameters->doubles, variety, "variete");
  fillMapWithDoubleList(parameters->doubles, plot, "parcelle");
  fillMapWithDoubleList(parameters->doubles, itinerary, "itinerairetechnique");
  fillMapWithDoubleList(parameters->doubles, station, "station");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  parameters->doubles.insert( pair<string ,pair <double, string> >( "startingdate",
                                                                    pair <double, string>(toJulianDayCalc(from_date), "simulation") ));
  parameters->doubles.insert( pair<string ,pair <double, string> >( "endingdate",
                                                                    pair <double, string>(toJulianDayCalc(to_date), "simulation") ));
  Samara samara;
  List result = resultToList(samara.run_samara_2_1(parameters));
  delete parameters;
  return result;
}


// [[Rcpp::export]]
List runSimpleSamara2_3(List params, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, params, "");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  Samara samara;
  pair <vector <string>, vector < vector <double> > > results = samara.run_samara_2_3(parameters);
  List result = resultToList(results);
  delete parameters;
  return result;
}

// [[Rcpp::export]]
List runSamara2_3(Rcpp::String from_date, Rcpp::String to_date, List simulation, List variety, List plot, List itinerary, List station, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, simulation, "simulation");
  fillMapWithDoubleList(parameters->doubles, variety, "variete");
  fillMapWithDoubleList(parameters->doubles, plot, "parcelle");
  fillMapWithDoubleList(parameters->doubles, itinerary, "itinerairetechnique");
  fillMapWithDoubleList(parameters->doubles, station, "station");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  parameters->doubles.insert( pair<string ,pair <double, string> >( "startingdate",
                                                                    pair <double, string>(toJulianDayCalc(from_date), "simulation") ));
  parameters->doubles.insert( pair<string ,pair <double, string> >( "endingdate",
                                                                    pair <double, string>(toJulianDayCalc(to_date), "simulation") ));
  Samara samara;
  List result = resultToList(samara.run_samara_2_3(parameters));
  delete parameters;
  return result;
}

// [[Rcpp::export]]
List runSimpleSamara2_3_lodging(List params, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, params, "");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  Samara samara;
  pair <vector <string>, vector < vector <double> > > results = samara.run_samara_2_3_lodging(parameters);
  List result = resultToList(results);
  delete parameters;
  return result;
}

// [[Rcpp::export]]
List runSamara2_3_lodging(Rcpp::String from_date, Rcpp::String to_date, List simulation, List variety, List plot, List itinerary, List station, List meteo)
{
  SamaraParameters * parameters = new SamaraParameters();
  fillMapWithDoubleList(parameters->doubles, simulation, "simulation");
  fillMapWithDoubleList(parameters->doubles, variety, "variete");
  fillMapWithDoubleList(parameters->doubles, plot, "parcelle");
  fillMapWithDoubleList(parameters->doubles, itinerary, "itinerairetechnique");
  fillMapWithDoubleList(parameters->doubles, station, "station");
  fillClimaticVectorWithList(parameters->climatics, meteo);
  parameters->doubles.insert( pair<string ,pair <double, string> >( "startingdate",
                                                                    pair <double, string>(toJulianDayCalc(from_date), "simulation") ));
  parameters->doubles.insert( pair<string ,pair <double, string> >( "endingdate",
                                                                    pair <double, string>(toJulianDayCalc(to_date), "simulation") ));
  Samara samara;
  List result = resultToList(samara.run_samara_2_3_lodging(parameters));
  delete parameters;
  return result;
}


// [[Rcpp::export]]
List rcpp_reduceVobs(List vObs, List results) {
  std::map <std::string, std::vector<double> > vObsMap;
  std::map <std::string, std::vector<double> > resultMap;
  vObsMap = mapFromDF(vObs);
  resultMap = mapFromDF(results);
  ResultParser parser;
  std::map<std::string, std::vector<double>>  ret = parser.filterVObs(vObsMap,resultMap, false);
  return mapOfVectorToDF(ret);
}





// List DFFromDoubleRecord(string table, string key, string value) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_record(table, key, value);
//     List result = DFFromDoubleMap(parameters->doubles, table);
//     delete parameters;
//     delete loader;
//     return result;
// }
//
//
// List DFFromStringRecord(string table, string key, string value) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_record(table, key, value);
//     List result = DFFromStringMap(parameters->strings, table);
//     delete parameters;
//     delete loader;
//     return result;
// }

// // [[Rcpp::export]]
// List DBSimulationParamsDF(Rcpp::String idsimulation) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_complete_simulation(idsimulation);
//     List result = DFFromDoubleMap(parameters->doubles, "");
//     delete parameters;
//     delete loader;
//     return result;
// }

// // [[Rcpp::export]]


// // [[Rcpp::export]]
// List DBSimulationMeteoDF(Rcpp::String idsimulation) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_complete_simulation(idsimulation);
//     List result = DFFromClimaticVector(parameters->climatics);
//     delete parameters;
//     delete loader;
//     return result;
// }
//
// // [[Rcpp::export]]
// List DBSimulationIdDF(Rcpp::String idsimulation) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_complete_simulation(idsimulation);
//     List result = DFFromStringMap(parameters->strings, "");
//     delete parameters;
//     delete loader;
//     return result;
// }
//
// // [[Rcpp::export]]
// List DBSimpleSimulationParamsDF(Rcpp::String idsimulation) {
//     return DFFromDoubleRecord("simulation","simcode",idsimulation);
// }
//
// // [[Rcpp::export]]
// List DBVarietyDF(Rcpp::String idvariete) {
//     return DFFromDoubleRecord("variety","variety",idvariete);
// }
//
// // [[Rcpp::export]]
// List DBStationDF(Rcpp::String codestation) {
//     return DFFromDoubleRecord("ws","wscode",codestation);
// }
//
// // [[Rcpp::export]]
// List DBPlotDF(Rcpp::String idparcelle) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_plot(idparcelle);
//     List result = DFFromDoubleMap(parameters->doubles, "parcelle");
//     delete parameters;
//     delete loader;
//     return result;
// }
//
// // [[Rcpp::export]]
// List DBItineraryDF(Rcpp::String iditinerairetechnique) {
//     return DFFromDoubleRecord("itinerairetechnique","iditinerairetechnique",iditinerairetechnique);
// }
//
// // [[Rcpp::export]]
// List DBMeteoDF(Rcpp::String codestation, Rcpp::String beginDate, Rcpp::String endDate) {
//     SamaraParameters * parameters = new SamaraParameters();
//     PSQLLoader * loader = new PSQLLoader(parameters);
//     loader->load_meteo(codestation, beginDate, endDate);
//     List result = DFFromClimaticVector(parameters->climatics);
//     delete parameters;
//     delete loader;
//     return result;
// }
//
// // [[Rcpp::export]]
// List DBObsDF(Rcpp::String idsimulation) {
//   SamaraParameters * parameters = new SamaraParameters();
//   PSQLLoader * loader = new PSQLLoader(parameters);
//   List result = mapOfVectorToDF(loader->load_obs(idsimulation));
//   delete parameters;
//   delete loader;
//   return result;
// }



