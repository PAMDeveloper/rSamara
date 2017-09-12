#ifndef RESULTPARSER_H
#define RESULTPARSER_H

class ResultParser
{
public:
    ResultParser() {

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

    map<string, vector<double>> filterVObs( map<string, vector<double>> vObs,
                                            map<string, vector<double> > results,
                                            bool keepDay = true,
                                            map<string, double> constraints = map<string,double>(),
                                            string dayId = "obsplantdate")
    {
        double dayMax = results.begin()->second.size();
        map<string, vector<double>> filteredVObs;
        for(auto const &token : vObs) {
            string * s = new string(token.first);
            transform(s->begin(), s->end(), s->begin(), ::tolower);
            if(results.find(*s) != results.end() || (keepDay && *s == dayId)) {
                filteredVObs.insert( pair<string,vector<double> >(*s, vector<double>()) );
            }
            delete s;
        }

        for (int i = 0; i < vObs[dayId].size(); ++i) {
            bool valid = true;

            valid &= vObs[dayId][i] <= dayMax;
            if(valid){
                for(auto const &token : filteredVObs)  {
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    if(constraints.find(*s) != constraints.end()) {
                        valid &= vObs[*s][i] == constraints[*s];
                    }
                    delete s;
                }
            }

            if(valid){
                for(auto token : filteredVObs) {
                    string * h = new string(token.first);
                    string * s = new string(token.first);
                    transform(s->begin(), s->end(), s->begin(), ::tolower);
                    filteredVObs[*s].push_back(vObs[*h][i]);
                    delete s;
                    delete h;
                }
            }
        }

        return filteredVObs;
    }


    map<string, vector<double>> reduceResults(map<string, vector<double> > results,
                                              map<string, vector<double> > vObs,
                                              map<string, double> constraints = map<string,double>(),
                                              string dayId = "obsplantdate") {

        map<string, vector<double>> filteredVObs = filterVObs(vObs, results, true, constraints, dayId);

        map<string, vector<double>> reducedResults;
        for(auto const &token : filteredVObs) {
            string s = token.first;
            transform(s.begin(), s.end(), s.begin(), ::tolower);
            if(results.find(s) != results.end()) {
                reducedResults.insert( pair<string,vector<double> >(s, vector<double>()) );
            }
        }


        for(auto const &r : reducedResults) {
            for (int i = 0; i < filteredVObs[dayId].size(); ++i) {
                int day = filteredVObs[dayId][i];
                if(day <= results[r.first].size())
                    reducedResults[r.first].push_back(results[r.first][day-1]);
            }
        }

        return reducedResults;
    }
};

#endif // RESULTPARSER_H
