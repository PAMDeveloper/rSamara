#ifndef RESULTPARSER_H
#define RESULTPARSER_H

#include <cmath>

class ResultParser
{
public:
    ResultParser()
    {

    }

    //1 seule view
//    map<string, vector<double>>  resultsToMap(EcomeristemSimulator * simulator) {
//        map<string, vector<double>> result;
//        const Observer& observer = simulator->observer();
//        const Observer::Views& views = observer.views();
//        Observer::Views::const_iterator it = views.begin();
//        View::Values values = it->second->values();
//        double begin = it->second->begin();
//        double end = it->second->end();

//        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
//            string s = itv->first;
//            transform(s.begin(), s.end(), s.begin(), ::tolower);
//            result.insert(std::pair<string,vector<double> >(s, vector<double>()) );
//        }

//        // write values
//        for (View::Values::const_iterator itv = values.begin(); itv != values.end(); ++itv) {
//            View::Value::const_iterator itp = itv->second.begin();
//            string s = itv->first;
//            transform(s.begin(), s.end(), s.begin(), ::tolower);

//            for (double t = begin; t <= end; ++t) {
//                while (itp != itv->second.end() and itp->first < t) {
//                    ++itp;
//                }

//                if (itp != itv->second.end()) {
//                    string c = itp->second;
//                    char* p;
//                    double converted = strtod(c.c_str(), &p);
//                    if (*p) {
//                        result[s].push_back(nan(""));
//                    } else {
//                        result[s].push_back(converted);
//                    }
//                } else {
//                    result[s].push_back(nan(""));
//                }
//            }
//        }
//        return result;
//    }

    map<string, vector<double>> filterVObs(map<string, vector<double>> vObs,
                                           map<string, vector<double>> results,
                                           bool keepDay = true,
                                           map<string, double> constraints = map<string, double>(),
                                           string dayId = "obsplantdate", bool lowerCase = true)
    {
        double dayMin = results[dayId].front();
        double dayMax = results[dayId].back();

        //delete obs columns
        map<string, vector<double>> filteredVObs;
        for (auto const& token : vObs) {
            string* s = new string(token.first);
            if (lowerCase)
                transform(s->begin(), s->end(), s->begin(), ::tolower);
            if (results.find(*s) != results.end() || (keepDay && *s == dayId)) {
                bool empty = true;
                for (double val : token.second) {
                    if (val != -999) {
                        empty = false;
                        break;
                    }
                }
                if (!empty)
                    filteredVObs.insert(pair<string, vector<double>>(*s, vector<double>()));
            }
            delete s;
        }

        for (int i = 0; i < vObs[dayId].size(); ++i) {
            bool valid = true;

            valid &= (vObs[dayId][i] <= dayMax) && (vObs[dayId][i] >= dayMin);
            /*if(valid){
                for(auto const &token : filteredVObs)  {
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    if(constraints.find(*s) != constraints.end()) {
                        valid &= vObs[*s][i] == constraints[*s];
                    }
                    delete s;
                }
            }*/

            //copy valid lines
            if (valid) {
                for (auto token : filteredVObs) {
                    string* h = new string(token.first);
                    string* s = new string(token.first);
                    if (lowerCase)
                        transform(s->begin(), s->end(), s->begin(), ::tolower);
                    filteredVObs[*s].push_back(vObs[*h][i]);
                    delete s;
                    delete h;
                }
            }
        }

        return filteredVObs;
    }


    map<string, vector<double>> reduceResults(map<string, vector<double>> results,
                                              map<string, vector<double>> vObs,
                                              map<string, double> constraints = map<string, double>(),
                                              string dayId = "obsplantdate", bool lowerCase = true)
    {

        double dayMin = results[dayId].front();
        double dayMax = results[dayId].back();

        map<string, vector<double>> filteredVObs = filterVObs(vObs, results, true, constraints, dayId, lowerCase);
        map<string, vector<double>> reducedResults;

        for (auto const& token : filteredVObs) {
            string s = token.first;
            if (lowerCase)
                transform(s.begin(), s.end(), s.begin(), ::tolower);
            if (results.find(s) != results.end()) {
                reducedResults.insert(pair<string, vector<double>>(s, vector<double>()));
            }
        }

        for (auto const& r : reducedResults) {
            for (int i = 0; i < filteredVObs[dayId].size(); ++i) {
                int day = filteredVObs[dayId][i];
                if (day == -1) {
                    day = dayMax;
                }
                if (day <= dayMax && day >= dayMin) {
                    int index = day - dayMin;
                    reducedResults[r.first].push_back(results[r.first][index]);
                }
            }
        }

        return reducedResults;
    }


    map<string, vector<double>> spl_ReduceResults(map<string, vector<double>> results,
                                                  map<string, vector<double>> vObs,
                                                  string colMatch = "nbjas")
    {
        double lastMatch = results[colMatch].back();

        map<string, vector<double>> res;

        for (auto const& token : vObs) {
            string s = token.first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            res.insert(pair<string, vector<double>>(s, vector<double>()));
        }

        int vObsIdx = 0;
        for (double m : vObs[colMatch]) {
            if (m == -1) {
                m = lastMatch;
            }
            auto idx = std::distance(results[colMatch].begin(),
                                     std::find(results[colMatch].begin(), results[colMatch].end(), m)
                                    );
            for (auto& r : res) {
                if( std::isnan(vObs[r.first][vObsIdx]) )
                    r.second.push_back(nan(""));
                else
                    r.second.push_back(results[r.first][idx]);
            }
            vObsIdx++;
        }

        return res;
    }
};

#endif // RESULTPARSER_H
