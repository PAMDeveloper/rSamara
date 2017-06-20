#ifndef SAMARA_DEFINES
#define SAMARA_DEFINES

#include <string>
#include <algorithm>
#include <cmath>
//#include <set>
//#include <map>
//#include <string>
//#include <sstream>
//#include <iostream>

#include "utils/julianconverter.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//rewrite delphi export
#define Q(x) #x
#define error_message(A, B) ; //std::cout << A << Q(x);
static double NilValue = -999;

static std::string FloatToStr(double val) {
  return std::to_string(val);
}

#endif
