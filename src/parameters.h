#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
#include <map>

using namespace std;

struct Climate {
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

  Climate(double TMax, double TMin, double TMoy,
          double HMax, double HMin, double HMoy,
          double Vt, double Ins, double Rg,
          double Rain, double ETP) :
    TMax(TMax), TMin(TMin), TMoy(TMoy), HMax(HMax), HMin(HMin),
    HMoy(HMoy), Vt(Vt), Ins(Ins), Rg(Rg), Rain(Rain), ETP(ETP)
  { }
};

class SamaraParameters {
 public:
    vector < Climate > climatics;
    map < string, pair < double, string > > doubles;
    map < string, pair < string, string > > strings;

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

    double getDouble(string s) {return doubles[s].first;}
    string getString(string s) {return strings[s].first;}
    Climate getClimate(int t){return climatics[t+1];}
};


#endif // PARAMETERS_H
