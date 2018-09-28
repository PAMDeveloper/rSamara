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
//struct PARAMS {
//   static const double min[11];
//   static const double max[11];
//   static const string names[11];
//   static const unsigned int size;
//};

//const unsigned int PARAMS::size = 11;
//const double PARAMS::min[11] = {1, 1, 1, 20, 10, 0.01, -0.5, 0.0, 0.0, 0.01, 0.1};
//const double PARAMS::max[11] = {10, 10, 20, 60, 120, 0.5, 0.5, 2, 0.5, 0.1, 0.5};
//const *double PARAMS::names[11] = {"Epsib", "Ict", "MGR_init", "plasto_init", "SLAp", "leaf_length_to_IN_length", "coef_MGR_PI", "slope_length_IN", "slope_LL_BL_at_PI", "density_IN1", "density_IN2"};

class SamaraFitness : public de::IOptimizable
{

public:
    SamaraParameters * parameters;
    vector<string> params;
    vector<pair<double, double> > bounds;
    map<string,vector<double>> rObs;

    SamaraFitness(SamaraParameters * parameters,
                  vector<string> params, vector<pair<double, double> > bounds,
                  map<string,vector<double>> obs):
        parameters(parameters), params(params), bounds(bounds) {
        ResultParser parser;
        Samara samara;
        pair <vector <string>, vector < vector <double> > > result = samara.run_samara_2_3(parameters, (Samara::SamaraLogType)1);
        map<string, vector<double>> mapRes;
        for(int i = 0; i < result.first.size(); i++) {
            mapRes.insert(pair<string,vector<double>>(result.first[i], result.second[i]));
        }

        rObs = parser.filterVObs(obs, mapRes, false, map<string,double>(), "ObsPlantDate", false);
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
//                qDebug() << diff << " ";
            }
//            qDebug() << "\n" << QString::fromStdString(key) << ": " << col << "\n";
//            qDebug() << flush;
            fitness += sqrt(col/vObs.size());
        }
//        qDebug() << fitness;
        return fitness;
    }

    double EvaluteCost(std::vector<double> inputs) const override
    {
        ResultParser parser;
        //put inputs in parameters
        for(int i = 0; i < inputs.size(); i++) {
            parameters->doubles[params[i]].first = inputs[i];
        }
        Samara samara;
        pair <vector <string>, vector < vector <double> > > result = samara.run_samara_2_3(parameters, (Samara::SamaraLogType)1);
        map<string, vector<double>> mapRes;
        for(int i = 0; i < result.first.size(); i++) {
            mapRes.insert(pair<string,vector<double>>(result.first[i], result.second[i]));
        }
        map<string, vector<double>> res = parser.reduceResults(mapRes, rObs, map<string,double>(), "ObsPlantDate", false);
        return RMSE(res, rObs);
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
