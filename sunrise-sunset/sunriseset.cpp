#include "sunriseset.h"

double SunRiseSet::FNday(int y, int m, int d, float h)
{
    long int luku = - 7 * (y + (m + 9)/12)/4 + 275*m/9 + d;

    // Typecasting needed for TClite on PC DOS at least, to avoid product overflow
    luku+= (long int)y*367;

    return (double)luku - 730531.5 + h/24.0;
}
double SunRiseSet::FNrange(double x)
{
    double b = x / tpi;
    double a = tpi * (b - (long)(b));
    if (a < 0) a = tpi + a;
    return a;
}

double SunRiseSet::f0(double lat, double declin)
{
    double fo,dfo;
    // Correction: different sign at S HS
    dfo = rads*(0.5*SunDia + AirRefr); if (lat < 0.0) dfo = -dfo;
    fo = tan(declin + dfo) * tan(lat*rads);

    if (fo > 0.99999) fo = 1.0; // to avoid overflow //
    fo = asin(fo) + pi/2.0;
    return fo;
}

double SunRiseSet::f1(double lat, double declin)
{
    double fi,df1;
    // Correction: different sign at S HS
    df1 = rads * 6.0; if (lat < 0.0) df1 = -df1;
    fi = tan(declin + df1) * tan(lat*rads);

    if (fi > 0.99999) fi = 1.0; // to avoid overflow //
    fi = asin(fi) + pi/2.0;
    return fi;
}

double SunRiseSet::FNsun(double d)
{
    // mean longitude of the Sun
    L = FNrange(280.461 * rads + .9856474 * rads * d);

    // mean anomaly of the Sun
    g = FNrange(357.528 * rads + .9856003 * rads * d);

    // Ecliptic longitude of the Sun
    return FNrange(L + 1.915 * rads * sin(g) + .02 * rads * sin(2 * g));
}

Clock SunRiseSet::gethrmn(double dhr)
{

    int hr,mn;

    hr = (int) dhr;
    mn = (dhr - (double) hr)*60;

    Clock ret(hr,mn);

    return ret;
}

int SunRiseSet::isItWinterTime()
{
    time_t sekunnit;
    struct tm *p;

    // get the date and time from the user
    // read system date and extract the year

    /** First get current time **/
    time(&sekunnit);

    /** Next get localtime **/

    p = localtime(&sekunnit);

    if(p->tm_isdst == 0)
        return 1;
    else if (p->tm_isdst == 1)
        return 2;
    else
        return -1;
}

SunRiseSet::SunRiseSet()
{

}

std::string SunRiseSet::getAllData()
{
    double y,m,day,h,latit,longit;

    time_t sekunnit;
    struct tm *p;

    // get the date and time from the user
    // read system date and extract the year

    /** First get current time **/
    time(&sekunnit);

    /** Next get localtime **/

    p = localtime(&sekunnit);
    // this is Y2K compliant algorithm
    y = 1900 + p->tm_year;

    m = p->tm_mon + 1;
    day = p->tm_mday;
    h = 12;

    std::cout << "Input latitude, longitude and timezone\n";
    latit = LATITUDE;
    longit = LONGITUDE;
    // Timezone hours
    double tzone= isItWinterTime();
    double d = FNday(y, m, day, h);

    // Use FNsun to find the ecliptic longitude of the
    // Sun
    double lambda = FNsun(d);

    // Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * d;

    // Find the RA and DEC of the Sun
    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));

    // Find the Equation of Time in minutes
    // Correction suggested by David Smith
    double LL = L - alpha;
    if (L < pi) LL += tpi;
    double equation = 1440.0 * (1.0 - LL / tpi);
    double ha = f0(latit,delta);
    double hb = f1(latit,delta);
   // double twx = hb - ha;   // length of twilight in radians
   // twx = 12.0*twx/pi;      // length of twilight in degrees

    // Conversion of angle to hours and minutes //
    daylen = degs * ha / 7.5;
    if (daylen<0.0001) {daylen = 0.0;}
    // arctic winter   //

    double riset = 12.0 - 12.0 * ha/pi + tzone - longit/15.0 + equation/60.0;
    double settm = 12.0 + 12.0 * ha/pi + tzone - longit/15.0 + equation/60.0;
    //double noont = riset + 12.0 * ha/pi;
    //double altmax = 90.0 + delta * degs - latit;
    // Correction suggested by David Smith
    // to express as degrees from the N horizon

    //if (delta * degs > latit ) altmax = 90.0 + latit - delta * degs;

   // double twam = riset - twx;    // morning twilight begin
   // double twpm = settm + twx;      // evening twilight end

    if (riset > 24.0) riset-= 24.0;
    if (settm > 24.0) settm-= 24.0;

    std::stringstream ss;
    ss << "\n Sunrise and set\n";
    ss << "===============\n";
    ss << "  year  : " << y << '\n';
    ss << "  month : " << m << '\n';
    ss << "  day   : " << day << "\n\n";
    ss << "Days until Y2K :  " << d << '\n';
    ss << "Latitude :  " << latit << ", longitude:  " << longit << '\n';
    ss << "Timezone :  " << tzone << "\n\n";
    ss << "Declination : " << delta * degs << '\n';
    ss << "Daylength   : "<< gethrmn(daylen).m_h << ":" << gethrmn(daylen).m_min << " hours \n";
  //  ss << "Begin civil twilight: "<<
  //        gethrmn(twam).m_h << ":" << gethrmn(twam).m_min  << '\n';

    ss << "Sunrise     : " << gethrmn(riset).m_h << ":" << gethrmn(riset).m_min << '\n';
    ss << "Sun altitude at noontime ";

 //   std::cout << "Cyniu twpm: " << gethrmn(twpm).getString() << std::endl;
 //   std::cout << "Cyniu settm: " << gethrmn(settm).getString() << std::endl;
    ss << "Sunset      : " <<
          gethrmn(settm).m_h << ":" << gethrmn(settm).m_min << '\n';
   // ss << "Civil twilight: " <<
   //       gethrmn(twpm).m_h << ":"  << gethrmn(twpm).m_min << '\n';
    return ss.str();
}

void SunRiseSet::setPosition(double LATITUDE, double LONGITUDE)
{
    this->LATITUDE = LATITUDE;
    this->LONGITUDE = LONGITUDE;
}

Clock SunRiseSet::getSunRise()
{
    double y,m,day,h,latit,longit;
    time_t sekunnit;
    struct tm *p;
    // get the date and time from the user
    // read system date and extract the year

    /** First get current time **/
    time(&sekunnit);

    /** Next get localtime **/
    p = localtime(&sekunnit);
    // this is Y2K compliant algorithm
    y = 1900 + p->tm_year;
    m = p->tm_mon + 1;
    day = p->tm_mday;
    h = 12;
    latit = LATITUDE;
    longit = LONGITUDE;
    // Timezone hours
    double tzone = isItWinterTime();
    double d = FNday(y, m, day, h);
    // Use FNsun to find the ecliptic longitude of the
    // Sun
    double lambda = FNsun(d);
    // Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * d;
    // Find the RA and DEC of the Sun
    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));
    double LL = L - alpha;
    if (L < pi) LL += tpi;
    double equation = 1440.0 * (1.0 - LL / tpi);
    double ha = f0(latit,delta);
    // Conversion of angle to hours and minutes //
    daylen = degs * ha / 7.5;
    if (daylen<0.0001) {daylen = 0.0;}
    // arctic winter   //
    double riset = 12.0 - 12.0 * ha/pi + tzone - longit/15.0 + equation/60.0;

    if (riset > 24.0) riset-= 24.0;
    return  gethrmn(riset);
}

Clock SunRiseSet::getDayLength()
{
    double y,m,day,h,latit;
    time_t sekunnit;
    struct tm *p;

    // get the date and time from the user
    // read system date and extract the year

    /** First get current time **/
    time(&sekunnit);

    /** Next get localtime **/
    p = localtime(&sekunnit);
    // this is Y2K compliant algorithm
    y = 1900 + p->tm_year;
    m = p->tm_mon + 1;
    day = p->tm_mday;
    h = 12;
    latit = LATITUDE;
    double d = FNday(y, m, day, h);

    // Use FNsun to find the ecliptic longitude of the
    // Sun
    double lambda = FNsun(d);
    // Obliquity of the ecliptic
    double obliq = 23.439 * rads - .0000004 * rads * d;
    double delta = asin(sin(obliq) * sin(lambda));
    // Find the Equation of Time in minutes
    // Correction suggested by David Smith
    double ha = f0(latit,delta);
    // Conversion of angle to hours and minutes //
    daylen = degs * ha / 7.5;
    if (daylen<0.0001) {daylen = 0.0;}
    // arctic winter   //

    return  gethrmn(daylen);
}

Clock SunRiseSet::getSunSet()
{
    double y,m,day,h,latit,longit;

    time_t sekunnit;
    struct tm *p;

    // get the date and time from the user
    // read system date and extract the year

    /** First get current time **/
    time(&sekunnit);

    /** Next get localtime **/

    p = localtime(&sekunnit);
    // this is Y2K compliant algorithm
    y = 1900 + p->tm_year;

    m = p->tm_mon + 1;
    day = p->tm_mday;
    h = 12;

    latit = LATITUDE;
    longit = LONGITUDE;
    // Timezone hours
    double tzone = isItWinterTime();

    double d = FNday(y, m, day, h);

    // Use FNsun to find the ecliptic longitude of the
    // Sun

    double lambda = FNsun(d);

    // Obliquity of the ecliptic

    double obliq = 23.439 * rads - .0000004 * rads * d;

    // Find the RA and DEC of the Sun

    double alpha = atan2(cos(obliq) * sin(lambda), cos(lambda));
    double delta = asin(sin(obliq) * sin(lambda));


    // Find the Equation of Time in minutes
    // Correction suggested by David Smith

    double LL = L - alpha;
    if (L < pi) LL += tpi;
    double equation = 1440.0 * (1.0 - LL / tpi);


    double ha = f0(latit,delta);

    // Conversion of angle to hours and minutes //
    daylen = degs * ha / 7.5;
    if (daylen<0.0001) {daylen = 0.0;}
    // arctic winter   //

    //double riset = 12.0 - 12.0 * ha/pi + tzone - longit/15.0 + equation/60.0;
    double settm = 12.0 + 12.0 * ha/pi + tzone - longit/15.0 + equation/60.0;

    //if (riset > 24.0) riset-= 24.0;
    if (settm > 24.0) settm-= 24.0;
    return  gethrmn(settm);
}
