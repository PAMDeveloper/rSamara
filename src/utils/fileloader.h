#ifndef FILELOADER_H
#define FILELOADER_H

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <QDebug>
#include "../parameters.h"

using namespace std;

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


#endif // FILELOADER_H
