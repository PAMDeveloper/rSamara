    #ifndef SAMARAFITNESS_H
#define SAMARAFITNESS_H

#include <utils/de.h>
#include <samara.h>
#include <utils/resultparser.h>
#include <parameters.h>
#include <ctime>
#include<cmath>

#include <QDebug>

using namespace std;

class SamaraFitness : public de::IOptimizable
{

public:
    vector <SamaraParameters *> parametersList;
    vector<string> params;
    vector<pair<double, double> > bounds;
    vector<map<string,vector<double>>> rObs;

    SamaraFitness(vector <SamaraParameters *> parametersList,
                  vector<string> params,
                  vector<pair<double, double> > bounds,
                  vector < map<string,vector<double>> > observations):
        parametersList(parametersList), params(params), bounds(bounds) {
        ResultParser parser;
        Samara samara;
        pair <vector <string>, vector < vector <double> > > result = samara.run_samara_2_3(parametersList[0], (Samara::SamaraLogType)1);
        map<string, vector<double>> mapRes;
        for(int i = 0; i < result.first.size(); i++) {
            mapRes.insert(pair<string,vector<double>>(result.first[i], result.second[i]));
        }

        for(auto obs: observations)
            rObs.push_back(parser.filterVObs2(obs, mapRes, false, map<string,double>(), "NbJas", false));
    }

    double RMSE(map<string,vector<double>> res, map<string,vector<double>> obs) const{
        double fitness = 0;
        for(auto token: res) {
            string key = token.first;
            vector<double> vRes = token.second;
            vector<double> vObs = obs.at(key);
            double col = 0;
            for(int i = 0; i < vObs.size(); i++) {
                if(vObs[i] == -999 || vObs[i] == 0) continue;
                double diff = pow(vObs[i] - vRes[i], 2);
                col += diff;
            }
            fitness += sqrt(col/vObs.size());
        }
        return fitness;
    }

    double EvaluteCost(std::vector<double> inputs) const override
    {
        ResultParser parser;
        //put inputs in parameters
        double cost = 0;
        for (int k = 0; k < parametersList.size(); ++k) {
            SamaraParameters * parameters = parametersList[k];
            for(int i = 0; i < inputs.size(); i++) {
                parameters->doubles[params[i]].first = inputs[i];
            }
            Samara samara;
            pair <vector <string>, vector < vector <double> > > result = samara.run_samara_2_3(parameters, (Samara::SamaraLogType)1);
            map<string, vector<double>> mapRes;
            for(int i = 0; i < result.first.size(); i++) {
                mapRes.insert(pair<string,vector<double>>(result.first[i], result.second[i]));
            }
            map<string, vector<double>> res = parser.reduceResults(mapRes, rObs[k], map<string,double>(), "NbJas", false);
            cost += RMSE(res, rObs[k]);
        }
        return cost;
    }

    unsigned int NumberOfParameters() const override {
        return (unsigned int)params.size();
    }

    std::vector<Constraints> GetConstraints() const override {
        std::vector<Constraints> constr(NumberOfParameters());

        for (unsigned int i = 0; i < NumberOfParameters(); ++i) {
            constr[i] = Constraints(bounds[i].first, bounds[i].second, true);
        }

        return constr;
    }
};
#endif // SAMARAFITNESS_H
