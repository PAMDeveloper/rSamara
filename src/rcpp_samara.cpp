#include <Rcpp.h>
#include <Rinternals.h>
#include "parameters.h"
#include "rsamara_types.hpp"
#include <iostream>
#include <fstream>

using namespace Rcpp;

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

 std::map <std::string, std::vector <double > > mapFromDF(DataFrame list) {
   std::map <std::string, std::vector <double > > map;
   CharacterVector names = list.attr("names");
   for (int i = 0; i < names.size(); ++i) {
     std::vector<double> vec = as<std::vector<double> >(list[i]);
     std::string s = Rcpp::as<string>(names(i));
     transform(s.begin(), s.end(), s.begin(), ::tolower);
     std::pair <std::string, std::vector<double> > token( s, vec );
     map.insert( token );
   }
   return map;
 }

 void fillMapWithDoubleList(std::map <std::string, std::pair <double, std::string> > & map, List list, std::string category) {
   CharacterVector names = list.attr("names");
   NumericVector values = list[0];
   for (int i = 0; i < names.size(); ++i) {
     double val = *REAL(list[i]);
     std::string key = Rcpp::as<std::string>(names[i]);
     std::pair <double, std::string> token( val, category );
     map.insert( std::pair<std::string, std::pair <double, std::string> >(
         key, token )
     );

   }
 }

 void fillClimaticVectorWithList(std::vector<Climate> & climatics, List list) {
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

 DataFrame resultToList(const std::pair <std::vector <std::string>, std::vector < std::vector <double> > > & results) {
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

 List DFFromDoubleMap(const std::map <std::string, std::pair <double, std::string> > & map, std::string category)
 {
   Rcpp::CharacterVector names;
   Rcpp::NumericVector values;
   for(auto const& it: map){
     std::string key = it.first;
     double value = it.second.first;
     std::string cat = it.second.second;
     if(cat == category || category == "") {
       names.push_back(key);
       values.push_back(value);
     }
   }
   DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
   return df;
 }

 List DFFromStringMap(const std::map <std::string, std::pair <std::string, std::string> > & map, std::string category)
 {
   Rcpp::CharacterVector names;
   Rcpp::CharacterVector values;
   for(auto const& it: map){
     std::string key = it.first;
     std::string value = it.second.first;
     std::string cat = it.second.second;
     if(cat == category || category == "") {
       names.push_back(key);
       values.push_back(value);
     }
   }
   DataFrame df = DataFrame::create(Named("Name")=names,Named("Values")=values);
   return df;
 }

 List DFFromClimaticVector(std::vector < Climate > meteoValues)
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

 SamaraParameters * current_params = nullptr;
 std::vector<SamaraParameters*> params_vector;

 SamaraParameters * params_sim(List params, List meteo, List str_params) {
   SamaraParameters * sparams = new SamaraParameters();

   CharacterVector names = params.attr("names");
   for (int i = 0; i < params.size(); ++i) {
     CharacterVector column = params[i];
     std::string key = Rcpp::as<string>(names[i]);
     if(column[0] != "") {
       double val = ::atof(column[0]);
       std::pair <double, std::string> token( val, "unestimated" );
       sparams->doubles.insert( std::pair<std::string, std::pair <double, std::string> >(key, token ));
     }
   }

   CharacterVector str_names = str_params.attr("names");
   for (int i = 0; i < str_params.size(); ++i) {
     CharacterVector column = str_params[i];
     std::string key = Rcpp::as<string>(str_names[i]);
     if(column[0] != "") {
       std::string val = Rcpp::as<std::string>(column[0]);
       std::pair <std::string, std::string> token( val, "unestimated" );
       sparams->strings.insert( std::pair<std::string, std::pair <std::string, std::string> >(key, token ));
     }
   }

   fillClimaticVectorWithList(sparams->climatics, meteo);

   return sparams;
 }


 //' Clean up simulation parameters.
 //' @export
 // [[Rcpp::export]]
 void clean() {
   if(current_params != nullptr)
     delete current_params;
   params_vector.clear();
 }

 //' Initialize simulation with given parameters.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @param str_params The string parameters.
 //' @export
 // [[Rcpp::export]]
 void init_sim(List params, List meteo, List str_params) {
   if(current_params != nullptr)
     delete current_params;

   current_params = params_sim(params, meteo, str_params);
 }

 //' Initialize simulation parameters without string parameters.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @return SamaraParameters* The initialized parameters.
 SamaraParameters * params_sim_simple(List params, List meteo) {
   SamaraParameters * sparams = new SamaraParameters();

   CharacterVector names = params.attr("names");
   for (int i = 0; i < params.size(); ++i) {
     CharacterVector column = params[i];
     std::string key = Rcpp::as<string>(names[i]);
     if(column[0] != "") {
       double val = ::atof(column[0]);
       std::pair <double, std::string> token( val, "unestimated" );
       sparams->doubles.insert( std::pair<std::string, std::pair <double, std::string> >(key, token ));
     }
   }

   fillClimaticVectorWithList(sparams->climatics, meteo);

   return sparams;
 }

 //' Initialize simulation at a given index without string parameters.
 //'
 //' @param idx The index for the simulation.
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @export
 // [[Rcpp::export]]
 void init_sim_idx_simple(int idx, List params, List meteo) {

   while(params_vector.size() < idx)
     params_vector.push_back(nullptr);

   if(params_vector[idx-1] != nullptr) {
     delete params_vector[idx-1];
   }

   params_vector[idx-1] = params_sim_simple(params, meteo);
 }

 //' Initialize simulation at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @param str_params The string parameters.
 //' @export
 // [[Rcpp::export]]
 void init_sim_idx(int idx, List params, List meteo, List str_params) {
   while(params_vector.size() < idx)
     params_vector.push_back(nullptr);

   if(params_vector[idx-1] != nullptr)
     delete params_vector[idx-1];

   params_vector[idx-1] = params_sim(params, meteo, str_params);
 }

 //' Update simulation parameters.
 //'
 //' @param params The simulation parameters.
 //' @param values The new values.
 //' @param names The names of the parameters to update.
 void update_params(SamaraParameters * params, NumericVector values, CharacterVector names) {
   for (int i = 0; i < names.size(); ++i) {
     double val = values[i];
     std::string key = Rcpp::as<string>(names[i]);
     std::pair <double, std::string> token( val, "estimation" );
     if(params->doubles.find(key) != params->doubles.end()) {
       params->doubles[key] = token;
     } else {
       params->doubles.insert( std::pair<std::string, std::pair <double, std::string> >(key, token) );
     }
   }
 }

 //' Update current simulation parameters.
 //'
 //' @param values The new values.
 //' @param names The names of the parameters to update.
 //' @export
 // [[Rcpp::export]]
 void update_sim(NumericVector values, CharacterVector names) {
   update_params(current_params, values, names);
 }

 //' Update simulation parameters at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @param values The new values.
 //' @param names The names of the parameters to update.
 //' @export
 // [[Rcpp::export]]
 void update_sim_idx(int idx, NumericVector values, CharacterVector names) {
   update_params(params_vector[idx-1], values, names);
 }

 //' Run simulation with given parameters.
 //'
 //' @param params The simulation parameters.
 //' @return List The simulation results.
 List run_params(SamaraParameters * params) {
   Samara samara;
   auto results = samara.run_samara_2_1(params);
   List result = resultToList(results);
   return result;
 }

 //' Run simulation with given parameters and version.
 //'
 //' @param params The simulation parameters.
 //' @param version The version of the simulation to run.
 //' @return List The simulation results.
 List run_params_version(SamaraParameters * params, int version) {
   Samara samara;
   if (version == 1) {
     auto results = samara.run_samara_2_1(params);
     List result = resultToList(results);
     return result;
   } else if (version == 2) {
     auto results = samara.run_samara_2_1_micha(params);
     List result = resultToList(results);
     return result;
   } else if (version == 3) {
     auto results = samara.run_samara_2_3(params);
     List result = resultToList(results);
     return result;
   } else if (version == 4) {
     auto results = samara.run_samara_2_3_lodging(params);
     List result = resultToList(results);
     return result;
   } else {
     auto results = samara.run_samara_2_3(params);
     List result = resultToList(results);
     return result;
   }
 }

 //' Run the current simulation.
 //'
 //' @return List The simulation results.
 //' @export
 // [[Rcpp::export]]
 List run_sim() {
   return run_params(current_params);
 }

 //' Run simulation at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @return List The simulation results.
 //' @export
 // [[Rcpp::export]]
 List run_sim_idx(int idx) {
   return run_params(params_vector[idx-1]);
 }

 //' Run simulation at a given index and version.
 //'
 //' @param idx The index for the simulation.
 //' @param version The version of the simulation to run.
 //' @return List The simulation results.
 //' @export
 // [[Rcpp::export]]
 List run_sim_idx_version(int idx, int version) {
   return run_params_version(params_vector[idx-1], version);
 }

 //' Reduce simulation results.
 //'
 //' @param results The simulation results.
 //' @param vobs The observed values.
 //' @return List The reduced results.
 //' @export
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

 //' Save simulation parameters to a file.
 //'
 //' @param params The simulation parameters.
 //' @param path The file path to save to.
 void save_params(SamaraParameters * params, string path) {
   ofstream file(path + ".csv");
   if (file.is_open())
   {
     file << params->strings.size() << "\n";
     for (auto const& token : params->strings) {
       file << token.first << "\t" << token.second.first << "\n";
     }

     file << params->doubles.size() << "\n";
     for (auto const& token : params->doubles) {
       file << fixed << token.first << "\t" << token.second.first << "\n";
     }

     file.close();
   } else {
     std::cout << "Unable to open file " << path << std::endl;
   }

   if ( ! file ) {
     std::cout << "Problem with file " << path << std::endl;
   }
 }

 //' Save current simulation parameters to a file.
 //'
 //' @param path The file path to save to.
 //' @export
 // [[Rcpp::export]]
 void save_sim(std::string path) {
   save_params(current_params, path);
 }

 //' Save simulation parameters at a given index to a file.
 //'
 //' @param idx The index for the simulation.
 //' @param path The file path to save to.
 //' @export
 // [[Rcpp::export]]
 void save_sim_idx(int idx, std::string path) {
   save_params(params_vector[idx-1], path);
 }

 //' Print string parameters of the simulation.
 //'
 //' @param params The simulation parameters.
 void print_sim_str_params(SamaraParameters * params) {
   for(auto const & t: params->strings) {
     std::cout << fixed << t.first << ": " << t.second.first << std::endl;
   }
 }

 //' Print double parameters of the simulation.
 //'
 //' @param params The simulation parameters.
 void print_sim_dbl_params(SamaraParameters * params) {
   for(auto const & t: params->doubles) {
     std::cout << fixed << t.first << ": " << t.second.first << std::endl;
   }
 }

 //' Print all parameters of the simulation.
 //'
 //' @param params The simulation parameters.
 void print_sim_params(SamaraParameters * params) {
   print_sim_str_params(params);
   print_sim_dbl_params(params);
 }

 //' Print string parameters of the current simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim_str() {
   print_sim_str_params(current_params);
 }

 //' Print double parameters of the current simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim_dbl() {
   print_sim_dbl_params(current_params);
 }

 //' Print all parameters of the current simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim() {
   print_sim_params(current_params);
 }

 //' Print string parameters of the simulation at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim_str_idx(int idx) {
   print_sim_str_params(params_vector[idx-1]);
 }

 //' Print double parameters of the simulation at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim_dbl_idx(int idx) {
   print_sim_dbl_params(params_vector[idx-1]);
 }

 //' Print all parameters of the simulation at a given index.
 //'
 //' @param idx The index for the simulation.
 //' @export
 // [[Rcpp::export]]
 void print_sim_idx(int idx) {
   print_sim_params(params_vector[idx-1]);
 }

 //' Get string parameters of the current simulation as a DataFrame.
 //'
 //' @return DataFrame The string parameters.
 //' @export
 // [[Rcpp::export]]
 DataFrame params_str_df() {
   return DFFromStringMap(current_params->strings, "");
 }

 //' Get string parameters of the simulation at a given index as a DataFrame.
 //'
 //' @param idx The index for the simulation.
 //' @return DataFrame The string parameters.
 //' @export
 // [[Rcpp::export]]
 DataFrame params_str_df_idx(int idx) {
   return DFFromStringMap(params_vector[idx-1]->strings, "");
 }

 //' Get double parameters of the current simulation as a DataFrame.
 //'
 //' @return DataFrame The double parameters.
 //' @export
 // [[Rcpp::export]]
 DataFrame params_dbl_df() {
   return DFFromDoubleMap(current_params->doubles, "");
 }

 //' Get double parameters of the simulation at a given index as a DataFrame.
 //'
 //' @param idx The index for the simulation.
 //' @return DataFrame The double parameters.
 //' @export
 // [[Rcpp::export]]
 DataFrame params_dbl_df_idx(int idx) {
   return DFFromDoubleMap(params_vector[idx-1]->doubles, "");
 }

 //' Check if the current simulation exists.
 //'
 //' @return int 1 if exists, 0 otherwise.
 //' @export
 // [[Rcpp::export]]
 int sim_exist() {
   return current_params != nullptr ? 1 : 0;
 }

 //' Check if the simulation at a given index exists.
 //'
 //' @param idx The index for the simulation.
 //' @return int 1 if exists, 0 otherwise.
 //' @export
 // [[Rcpp::export]]
 int sim_exist_idx(int idx) {
   return (params_vector.size() >= idx && params_vector[idx-1] != nullptr) ? 1 : 0;
 }

 //' Reduce results with observed values.
 //'
 //' @param results The simulation results.
 //' @param vobs The observed values.
 //' @return List The reduced results.
 //' @export
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

 //' Convert a date to Julian day using specified format and separator.
 //'
 //' @param date The date string.
 //' @param format The date format.
 //' @param sep The separator.
 //' @return double The Julian day.
 //' @export
 // [[Rcpp::export]]
 double toJulianDayCalcC(Rcpp::String date, Rcpp::String format, Rcpp::String sep) {
   return JulianCalculator::toJulianDay(date, format, sep);
 }

 //' Convert a date to Julian day using default format.
 //'
 //' @param date The date string.
 //' @return double The Julian day.
 //' @export
 // [[Rcpp::export]]
 double toJulianDayCalc(Rcpp::String date) {
   return JulianCalculator::toJulianDay(date, JulianCalculator::YMD, '-');
 }

 //' Convert a date to access format.
 //'
 //' @param date The date string.
 //' @param format The date format.
 //' @param sep The separator.
 //' @return std::string The date in access format.
 //' @export
 // [[Rcpp::export]]
 std::string toAccessFormat(Rcpp::String date, Rcpp::String format, Rcpp::String sep) {
   double jDay = JulianCalculator::toJulianDay(date, format, sep);
   return JulianCalculator::toStringDate(jDay, JulianCalculator::YMD, '/');
 }

 //' Run simulation and return results as DataFrame.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results as DataFrame.
 //' @export
 // [[Rcpp::export]]
 List run2DF(List params, List meteo)
 {
   SamaraParameters * parameters = new SamaraParameters();
   fillMapWithDoubleList(parameters->doubles, params, "");
   fillClimaticVectorWithList(parameters->climatics, meteo);
   Samara samara;

   auto results = samara.run_samara_2_1(parameters);
   List result = resultToList(results);
   delete parameters;
   return result;
 }

 //' Run simulation with detailed parameters and return results.
 //'
 //' @param from_date The start date.
 //' @param to_date The end date.
 //' @param simulation The simulation parameters.
 //' @param variety The variety parameters.
 //' @param plot The plot parameters.
 //' @param itinerary The itinerary parameters.
 //' @param station The station parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run a simple simulation in version 2.1 and return results.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run simulation in version 2.1 with detailed parameters and return results.
 //'
 //' @param from_date The start date.
 //' @param to_date The end date.
 //' @param simulation The simulation parameters.
 //' @param variety The variety parameters.
 //' @param plot The plot parameters.
 //' @param itinerary The itinerary parameters.
 //' @param station The station parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run a simple simulation in version 2.3 and return results.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run simulation in version 2.3 with detailed parameters and return results.
 //'
 //' @param from_date The start date.
 //' @param to_date The end date.
 //' @param simulation The simulation parameters.
 //' @param variety The variety parameters.
 //' @param plot The plot parameters.
 //' @param itinerary The itinerary parameters.
 //' @param station The station parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run a simple simulation in version 2.3 with lodging and return results.
 //'
 //' @param params The numeric parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Run simulation in version 2.3 and lodging with detailed parameters and return results.
 //'
 //' @param from_date The start date.
 //' @param to_date The end date.
 //' @param simulation The simulation parameters.
 //' @param variety The variety parameters.
 //' @param plot The plot parameters.
 //' @param itinerary The itinerary parameters.
 //' @param station The station parameters.
 //' @param meteo The meteorological data.
 //' @return List The simulation results.
 //' @export
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

 //' Reduce observed values with simulation results.
 //'
 //' @param vObs The observed values.
 //' @param results The simulation results.
 //' @return List The reduced observed values.
 //' @export
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
