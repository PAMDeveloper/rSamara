#ifndef JULIANCALCULATOR_H
#define JULIANCALCULATOR_H

#include <sstream>
#include <iostream>
#include <cstring>

using namespace std;
class JulianCalculator
{
    static const bool GREGORIAN_CALENDAR = true;

public:
    enum DateFormat { DMY, DYM, MYD, MDY, YDM, YMD };

    static int toJulianDay(string date, DateFormat format, char sep) {
//        date = date.substr(10);
        return (int)round(getJulianDay(parseString(date, format, sep)));
    }

    static int toJulianDay(string date, string format, string sep) {
        char cSep = sep.c_str()[0];
        DateFormat dFormat;
        if(format == "dmy" || format == "DMY") dFormat = DMY;
        if(format == "dym" || format == "DYM") dFormat = DYM;
        if(format == "myd" || format == "MYD") dFormat = MYD;
        if(format == "mdy" || format == "MDY") dFormat = MDY;
        if(format == "ydm" || format == "YDM") dFormat = YDM;
        if(format == "ymd" || format == "YMD") dFormat = YMD;
        return toJulianDay(date, dFormat, cSep);
    }

    static string toStringDate(double julianDay, DateFormat format, char sep){
        return getStringDate(getDate(julianDay), format, sep);
    }

    static int dayNumber(double julianDay) {
        return getDayOfYear(getDate(julianDay));
    }

// private:
    struct dateInts {
        dateInts(double year, double month, double day):
            year(year), month(month), day(day){}
        double year;
        double month;
        double day;
    };

    static int* monthsArray(dateInts date){
        int year = (int)date.year;
        int leap = (
            GREGORIAN_CALENDAR && (year % 4 == 0) ? 1 :
            (year % 4 != 0 ? 0 :
            (year % 400 == 0 ? 1 :
            (year % 100 == 0 ? 0 :
            1))));

        int * monthLengthArray = new int[12];
        monthLengthArray[0] = 31;
        monthLengthArray[1] = 28 + leap;
        monthLengthArray[2] = 31;
        monthLengthArray[3] = 30;
        monthLengthArray[4] = 31;
        monthLengthArray[5] = 30;
        monthLengthArray[6] = 31;
        monthLengthArray[7] = 31;
        monthLengthArray[8] = 30;
        monthLengthArray[9] = 31;
        monthLengthArray[10] = 30;
        monthLengthArray[11] = 31;
        return monthLengthArray;
    }

    static int getDayOfYear(dateInts date) {
        int nbDays = 0;
        int * months = monthsArray(date);
        for (int i = 0; i < date.month - 1; i++) {
            nbDays += months[i];
        }
        return nbDays + (int)date.day;
    }

    static dateInts parseString(string date, DateFormat format, char sep) {
        char * str = (char*)date.c_str();
        replace( str, str+strlen(str), sep, ' ' );
        double year, month ,day;
        switch (format) {
        case DMY:
            istringstream( str ) >> day >> month >> year;
            break;
        case DYM:
            istringstream( str ) >> day >> year >> month;
            break;
        case MYD:
            istringstream( str ) >> month >> year >> day;
            break;
        case MDY:
            istringstream( str ) >> month >> day >> year;
            break;
        case YDM:
            istringstream( str ) >> year >> day >> month;
            break;
        case YMD:
            istringstream( str ) >> year >> month >> day;
            break;
        default:
            break;
        }
        return dateInts(year, month, day);
    }

    static string getStringDate(dateInts date, DateFormat format, char sep) {
        string year = to_string((int)date.year);
        string month = (date.month < 10 ? "0" : "") + to_string((int)date.month);
        string day = (date.day < 10 ? "0" : "") + to_string((int)date.day);
        switch (format) {
        case DMY:
            return day + sep + month + sep + year;
            break;
        case DYM:
            return day + sep + year + sep + month;
            break;
        case MYD:
            return month + sep + year + sep + day;
            break;
        case MDY:
            return month + sep + day + sep + year;
            break;
        case YDM:
            return year + sep + day + sep + month;
            break;
        case YMD:
            return year + sep + month + sep + day;
            break;
        default:
            break;
        }
        return year + sep + month + sep + day;
    }

    static double getJulianDay(dateInts date)
    {
        double D = date.day;
        double M = date.month;
        double Y = date.year;
        if (M<3)	{
            Y--;
            M += 12;
        }
        double A, B, C;
        if(GREGORIAN_CALENDAR)	{
            A = floor(Y/100);
            B = floor(A/4);
            C = 2 - A + B;
        } else {
            C = 0;
        }

        double E = floor(365.25*(Y + 4716));
        double F = floor(30.6001*(M + 1));
        double julianday = C + D + E + F - 1524.5;
        return julianday;
    }

    static dateInts getDate(double julianDay)
    {
        double Z = julianDay + 0.5;
        double F = Z - floor(Z);
        double W, X, A, B, C, D, E;

        if(GREGORIAN_CALENDAR)	{
            Z = floor(Z);
            W = floor((Z - 1867216.25)/36524.25);
            X = floor(W/4);
            A = Z + 1 + W - X;
        } else {
            A = Z;
        }

        B = A + 1524;
        C = floor((B - 122.1)/365.25);
        D = floor(365.25*C);
        E = floor((B - D)/30.6001);

        double month = E > 13 ? E-13 : E-1;
        double day = B - D - floor(30.6001*E) +F;
        double year = month < 3 ? C-4715: C-4716;

        return dateInts(floor(year), floor(month), floor(day));
    }
};

#endif // JULIANCALCULATOR_H
