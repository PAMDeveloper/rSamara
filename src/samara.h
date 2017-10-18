#ifndef SAMARA_H
#define SAMARA_H

#include "samara_defines.h"
#include "parameters.h"

pair <vector <string>, vector < vector <double> > > run_samara_2_1(SamaraParameters * parameters);
pair <vector <string>, vector < vector <double> > > run_samara_2_3(SamaraParameters * parameters);

void set_meteo_vars(SamaraParameters * parameters, double t, double &TMax, double &TMin, double &TMoy
                    , double &HMax, double &HMin, double &HMoy
                    , double &Vt, double &Ins, double &Rg, double &ETP
                    , double &Pluie, double & TMoyCalc, double & HMoyCalc);
void init_parameters(SamaraParameters * params);
void init_psql_parameters(SamaraParameters * params);
void init_parcelle();
void eval_Par(double t);
void EToFao(double const &ETP, double const &Alt, double const &RgMax, double const &RgCalc,
            double const &TMin, double const &TMax,
            double const &HMin, double const &HMax, double const &HMoy,
            double const &TMoy, double const &Vt, double &ETo,
            double &TMoyPrec, double &VPDCalc);
void kill_crop();
void kill_crop2_2();
void reset_variables();
void init_culture();

#endif // SAMARA_H
