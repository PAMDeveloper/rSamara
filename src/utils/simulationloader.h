#ifndef SIMULATIONLOADER_H
#define SIMULATIONLOADER_H

#include "../parameters.h"

using namespace std;

class AbstractSimulationLoader
{
public:
    SamaraParameters * parameters;

    explicit AbstractSimulationLoader(SamaraParameters * params) {
        parameters = params;
    }

    virtual void load_complete_simulation(string idsimulation) = 0;
    virtual void load_simulation(string idsimulation) = 0;
    virtual void load_variety(string idvariete) = 0;
    virtual void load_station(string codestation) = 0;
    virtual void load_plot(string idparcelle) = 0;
    virtual void load_itinerary(string iditinerairetechnique) = 0;
    virtual void load_meteo(string codestation, string start, string end) = 0;
    virtual void load_meteo(string codestation, double startJulian, double endJulian) = 0;

    virtual vector<string> load_simulation_list() = 0;
    virtual vector<string>  load_variety_list() = 0;
    virtual vector<string>  load_station_list() = 0;
    virtual vector<string> load_plot_list() = 0;
    virtual vector<string>  load_itinerary_list() = 0;
};

#endif // SIMULATIONLOADER_H
