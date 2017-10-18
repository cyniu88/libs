#ifndef SUNRISESET_H
#define SUNRISESET_H

#include <math.h>
#include <time.h>
#include <iostream>
#include "../useful/useful.h"


class SunRiseSet
{
    int TIMEZONE  =  2;  //summer time 2 winter time 1 ?  check it  //TODO
    double LATITUDE  =  50.019519;
    double LONGITUDE =  19.903498;   //cracow
    double pi = M_PI;
    double tpi = 2 * pi;
    double degs = 180.0/pi;
    double rads = pi/180.0;
    double L = 0;
    double g = 0;
    double daylen = 0;
    double SunDia = 0.53;  // Sunradius degrees
    double AirRefr = 34.0/60.0; // athmospheric refraction degrees //

    // Get the days to J2000
    // h is UT in decimal hours
    // FNday only works between 1901 to 2099 - see Meeus chapter 7

    double FNday (int y, int m, int d, float h) ;

    // the function below returns an angle in the range
    // 0 to 2*pi

    double FNrange (double x) ;

    // Calculating the hourangle
    double f0(double lat, double declin);

    // Calculating the hourangle for twilight times
    //
    double f1(double lat, double declin);

    // Find the ecliptic longitude of the Sun
    double FNsun (double d);

    // Display decimal hours in hours and minutes
    Clock gethrmn(double dhr);

public:
    SunRiseSet();
    void printAllData();
    void setPosition(double LATITUDE , double LONGITUDE, int TIMEZONE );
    int getYear();
    int getMounth();
    int getDay();
    Clock getSunSet();
    Clock getSunRise();
    Clock getDayLength();
};

#endif // SUNRISESET_H
