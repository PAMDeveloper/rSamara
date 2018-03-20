#pragma once

#include <parameters.h>

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class TextLoader {
	static vector<string> split(const string& s, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(s);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
public:
	static void saveParameters(SamaraParameters * params, string path, char sep) {

	}

	static void saveMeteo(SamaraParameters * params, string path, char sep) {

	}

	static void loadParameters(SamaraParameters * params, string path, char sep) {
		ifstream infile(path);

		int rowCt;
		string line;
		infile >> rowCt;
		getline(infile, line);
		for (int row = 0; row < rowCt; ++row) {
			getline(infile, line);
			vector<string> lstLine = split(line, sep);
			params->strings.insert(pair<string, pair < string, string > >(
				lstLine[0],
				pair<string, string>(lstLine[1], "test")
				));
		}

		infile >> rowCt;
		getline(infile, line);
		for (int row = 0; row < rowCt; ++row) {
			getline(infile, line);
			vector<string> lstLine = split(line, sep);
			params->doubles.insert(pair<string, pair < double, string > >(
				lstLine[0],
				pair<double, string>(stod(lstLine[1]), "test")
				));
		}
	}

	static void loadMeteo(SamaraParameters * params, string path, char sep) {
		ifstream infile(path);
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
		string line;
		while (getline(infile, line))
		{
			istringstream iss(line);
			iss >> TMax >> TMin >> TMoy >> HMax >> HMin >> HMoy >> Vt >> Ins >> Rg >> Rain >> ETP;
			Climate c(TMax,TMin,TMoy,HMax,HMin,HMoy,Vt,Ins,Rg,Rain,ETP);
			params->climatics.push_back(c);
		}
	}
};
