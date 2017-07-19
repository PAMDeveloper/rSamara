#ifndef FILELOADER_H
#define FILELOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "simulationloader.h"
#include "../parameters.h"

#include <QDebug>
using namespace std;


class FileLoader : public AbstractSimulationLoader {
public:
    string folder;
    FileLoader(string folderPath, SamaraParameters * params)
        : AbstractSimulationLoader(params), folder(folderPath){

    }

    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }


    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    void load_file_parameters(string file_path, string id) {
        std::ifstream infile(file_path);
        std::string line;
        std::getline(infile, line); //config

        std::getline(infile, line); //headers
        vector<string> headers;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(headers));

        foreach (string s, headers) {
            qDebug() << QString::fromStdString(s);
        }

        while (std::getline(infile, line))
        {
            vector<string> data = split(line, '\t');
            foreach (string s, data) {
                qDebug() << QString::fromStdString(s);
            }
        }
    }



    void load_complete_simulation(string idsimulation){
        load_simulation(idsimulation);
        load_variety(parameters->getString("idvariete"));
        load_station(parameters->getString("codestation"));
        load_plot(parameters->getString("idparcelle"));
        load_itinerary(parameters->getString("iditinerairetechnique"));
        load_meteo(parameters->getString("codestation"),
                   parameters->getString("datedebut"),
                   parameters->getString("datefin"));
    }

//    void load_simulation(string idsimulation) {
//        load_record(query("Simule","Id",idsimulation), "simulation");
//    }

//    void load_variety(string idvariete) {
//        load_record(query("Variete","Id",idvariete), "variete");
//    }

//    void load_station(string codestation) {
//        load_record(query("Site","Id",codestation), "station");
//    }

//    void load_plot(string idparcelle) {
//        load_record(query("Parcelle","Id",idparcelle), "parcelle");
//        load_record(query("TypeSol","Id",parameters->getString("codetypesol")), "parcelle");
//    }

//    void load_itinerary(string iditinerairetechnique) {
//        load_record(query("ItineraireTechnique","Id",iditinerairetechnique), "itinerairetechnique");
//    }

//    void load_meteo(string codestation, string start, string end) {
//        parameters->clearMeteo();
//        start = JulianDayConverter::toJulianDay( JulianDayConverter::toJulianDayNumber(start) - 1);
//        std::string meteo_query = query("meteo","codestation",codestation) +
//                " AND jour >= '" + start + "' AND jour <= '" +
//                end + "' ORDER BY jour ASC";
//        load_records(meteo_query);
//        std::cout << meteo_query << std::endl;
//    }

//    void load_meteo(string codestation, double startJulian, double endJulian) {
//        parameters->clearMeteo();
//        string start = JulianDayConverter::toJulianDayFmt( startJulian - 1, DATE_FORMAT_YMD);
//        string end = JulianDayConverter::toJulianDayFmt( endJulian, DATE_FORMAT_YMD );
//        std::string meteo_query = query("meteo","codestation",codestation) +
//                " AND jour >= '" + start + "' AND jour <= '" +
//                end + "' ORDER BY jour ASC";
//        load_records(meteo_query);
//        std::cout << meteo_query << std::endl;
//    }


    vector<string> load_simulation_list(){}
    vector<string>  load_variety_list(){}
    vector<string>  load_station_list(){}
    vector<string> load_plot_list(){}
    vector<string>  load_itinerary_list(){}
};


#endif // FILELOADER_H
