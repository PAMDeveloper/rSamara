#ifndef SAMARA_H
#define SAMARA_H

#include "samara_defines.h"

#include "samara_parameters.h"
#include "samara_variables.h"

#include "utils/juliancalculator.h"


class Samara : public Samara_Attributes, public Samara_Variables {

public:
	pair <vector <string>, vector < vector <double> > > run_samara_2_1(SamaraParameters * parameters, SamaraLogType log = COMPLETE);
    pair <vector <string>, vector < vector <double> > > run_samara_2_1_micha(SamaraParameters * parameters, SamaraLogType log = COMPLETE);
    pair <vector <string>, vector < vector <double> > > run_samara_2_3(SamaraParameters * parameters, SamaraLogType log = COMPLETE);
    pair <vector <string>, vector < vector <double> > > run_samara_2_3_lodging(SamaraParameters * parameters, SamaraLogType log = COMPLETE);
    pair <vector <string>, vector < vector <double> > > run_samara_2_3_lodging_test(SamaraParameters * parameters, SamaraLogType log = COMPLETE);

    void set_meteo_vars(SamaraParameters * parameters, int t, double &TMax, double &TMin, double &TMoy
                        , double &HMax, double &HMin, double &HMoy
                        , double &Vt, double &Ins, double &Rg, double &ETP
                        , double &Pluie, double & TMoyCalc, double & HMoyCalc, double & Irrigation);

    void init_parameters_2_1(SamaraParameters * params);
    void init_parameters_2_1_micha(SamaraParameters * params);
    void init_parameters_2_3(SamaraParameters * params);

	void init_all_variables_2_1();
	void init_all_variables_2_1_micha();
	void init_all_variables_2_3();

	void reset_variables_2_1();
	void reset_variables_2_1_micha();
	void reset_variables_2_3();

	void init_culture();

    void init_parcelle(double &VolMacropores, double &HumSat, double &HumFC, double &ResUtil
                       , double &ProfRu, double &EpaisseurSurf, double &EpaisseurProf
                       , double &RuSurf, double &CapaREvap, double &CapaRFE, double &CapaRDE
                       , double &StRuMax, double &StockIniProf, double &StockIniSurf
                       , double &ValRSurf, double &ValRDE, double &ValRFE
                       , double &StockSurface, double &StockTotal, double &Hum
                       , double &LTRkdfcl, double &LIRkdf, double &LTRkdf
                       , double &CounterNursery, double &VolRelMacropores);

    void eval_Par(double t);

    void EToFao(double const &ETP, double const &Alt, double const &RgMax, double const &RgCalc,
                double const &TMin, double const &TMax,
                double const &HMin, double const &HMax, double const &HMoy,
                double const &TMoy, double const &Vt, double &ETo,
                double &TMoyPrec, double &VPDCalc);

    void EToFao_2_3(double const &ETP, double const &Alt, double const &RgMax, double const &RgCalc,
                    double const &TMin, double const &TMax,
                    double const &HMin, double const &HMax, double const &HMoy,
                    double const &TMoy, double const &Vt, double &ETo,
                    double &TMoyPrec, double &VPDCalc);

    void kill_crop();



};
#endif // SAMARA_H
