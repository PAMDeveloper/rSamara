#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <map>
#include <iostream>
#include <string>
using namespace std;

struct Climate {
    double JulianDay;
    double TMax;
    double TMin;
    double TMoy;
    double HMax;
    double HMin;
    double HMoy;
    double Vt;
    double Ins;
    double Rg;
    double Rain;
    double ETP;
    Climate(){}
    Climate(/*double JulianDay, */double TMax, double TMin, double TMoy,
            double HMax, double HMin, double HMoy,
            double Vt, double Ins, double Rg,
            double Rain, double ETP) :
        /*JulianDay(JulianDay),*/TMax(TMax), TMin(TMin), TMoy(TMoy), HMax(HMax), HMin(HMin),
        HMoy(HMoy), Vt(Vt), Ins(Ins), Rg(Rg), Rain(Rain), ETP(ETP)
    { }

    string c_to_string() {
      string s = "";
      s += " " + to_string(TMax);
      s += " " + to_string(TMin);
      s += " " + to_string(TMoy);
      s += " " + to_string(HMax);
      s += " " + to_string(HMin);
      s += " " + to_string(HMoy);
      s += " " + to_string(Vt);
      s += " " + to_string(Ins);
      s += " " + to_string(Rg);
      s += " " + to_string(Rain);
      s += " " + to_string(ETP);
      return s;

    }
};

class SamaraParameters {
public:
    vector < Climate > climatics;
    vector < double > irrigation;

    map < string, pair < double, string > > doubles;
    map < string, pair < string, string > > strings;

    void clearParameters() {
        doubles.clear();
        strings.clear();
    }

    void clearParameters(string s) {
        for(auto it = doubles.begin(), ite = doubles.end(); it != ite;) {
            if(it->second.second == s) it = doubles.erase(it);
            else ++it;
        }

        for(auto it = strings.begin(), ite = strings.end(); it != ite;) {
            if(it->second.second == s) it = strings.erase(it);
            else ++it;
        }
    }

    void clearMeteo() {climatics.clear();}
	void clearIrrigation() { irrigation.clear(); }

    double getDouble(string s) {
        if(doubles.find(s) == doubles.end()) {
            std::cout << "Missing variable: " << s << std::endl;
            return -999;
        }
        if( doubles[s].first != doubles[s].first ) {
            std::cout << "NAN" << s;
            return -999;
        }
        return doubles[s].first;
    }
    string getString(string s) {return strings[s].first;}

    Climate getClimate(int t){return climatics[t + 1];}
	double getIrrigation(int t) {
		if (t >= irrigation.size())
			return -999.0;
		return irrigation[t + 1];
	}
};


#endif // PARAMETERS_H
