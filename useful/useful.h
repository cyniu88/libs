#ifndef Iusefull_H
#define Iusefull_H

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <chrono>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#ifndef ANDROID
#include "json.hpp"
#endif

std::vector<std::string> split_string(const std::string& s, char separator );

class useful_F_libs {
public:
    static  void write_to_mkfifo(std::string path,std::string msg);
    static  std::string read_from_mkfifo(std::string path);
    static size_t  WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static std::string find_tag (const std::string &temp);
    //////////////////// HTTP req //////////////////////////
    static std::string httpPost(std::string url, int timeoutSeconds);
    static std::string httpPost(std::string url);
    static void downloadFile(std::string url, std::string path, int timeoutSeconds);
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    static std::string removeHtmlTag(std::string &data);
    /////////////////////  JSON ////////////////////////////

#ifndef ANDROID
    static nlohmann::json getJson(std::string url);
#endif
};
namespace std {

#ifdef ANDROID
template <typename T>
int stoi(T s){
    return atoi(s.c_str());
}
#endif

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 4)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

struct Clock{
    unsigned int h = 0;
    unsigned int min = 0;
    Clock () {}
    Clock(std::string t){
        std::vector<std::string> vt = split_string(t,':');
        int h = std::stoi(vt.at(0));
        int m = std::stoi(vt.at(1));
        set(static_cast <unsigned int>(h),static_cast <unsigned int>(m));
    }

    Clock(unsigned int h, unsigned int m) {
        set(h,m);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    void set(unsigned int h, unsigned int m){
        if (h<24 && m <60){
            this->h = h;
            this->min = m;
        }
        else {
            throw 0;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    const std::string getString(){
        std::stringstream ret;
        if (h < 10) {
            ret << "0";
        }
        ret << h;
        ret << ":";
        if (min < 10) {
            ret << "0";
        }
        ret << min;
        return ret.str();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const Clock & c){
        if ((this->h == c.h) && (this->min == c.min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const Clock & c){
        if ((this->h != c.h) || (this->min != c.min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    friend std::ostream & operator<< (std::ostream &w ,  Clock &c) {
        return w << c.getString();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator < (const Clock& c){
        if (this->h < c.h){
            return true;
        }
        else{
            if (this->h == c.h && this->min < c.min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator > (const Clock& c){
        if (this->h > c.h){
            return true;
        }
        else{
            if (this->h == c.h && this->min > c.min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator >= (const Clock& c){
        if (this->h > c.h){
            return true;
        }
        else if (this->h == c.h){

            if (this->min >= c.min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator <= (const Clock& c){
        if (this->h < c.h){
            return true;
        }
        else if (this->h == c.h){

            if (this->min <= c.min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    Clock  operator + (const Clock& c){
        unsigned int minutes, hours;
        minutes = min+ c.min;
        hours = h + c.h;
        if (minutes >59){
            minutes =  minutes % 60 ;
            hours+=1;
        }
        if (hours >= 24){
            hours-=24;
        }
        return  Clock(hours, minutes);

    }
    /////////////////////////////////////////////////////////////////////////////////////
    Clock&  operator += (const Clock& c){
        unsigned int minutes, hours;
        minutes = min+ c.min;
        hours = h + c.h;
        if (minutes >59){
            minutes =  minutes % 60 ;
            hours+=1;
        }
        if (hours >= 24){
            hours-=24;
        }
        this->h = hours;
        this->min = minutes;
        return *this;

    }
    /////////////////////////////////////////////////////////////////////////////////////

    unsigned int toSeconds(){
        return toSeconds(Clock(this->h, this->min) );
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static unsigned int toSeconds(Clock t){
        return ((t.h*60) + t.min)*60;
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static Clock fromSeconds(unsigned int sec){
        unsigned int h = sec/3600;
        unsigned int min = sec%3600;
        min = min/60;
        return Clock(h,min);
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static Clock periodOfTime(Clock start, Clock end)
    {
        if (end >= start){
            return Clock::fromSeconds(end.toSeconds() - start.toSeconds()  );
        }
        else{
            return Clock::fromSeconds(end.toSeconds() + ( Clock::toSeconds(Clock(23,59))+ 60 - start.toSeconds() ) );
        }
        //return diff;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    static Clock getTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return Clock( static_cast <unsigned int>(ltm->tm_hour),static_cast <unsigned int>(ltm->tm_min) );
    }
    /////////////////////////////////////////////////////////////////////////////////////
};

enum class STATE { // WARNING remember add new state to stateToString() usefull.cpp
    OFF,
    ON,
    UNKNOWN,
    PLAY,
    PAUSE,
    STOP,
    ACTIVE,
    DEACTIVE,
    WORKING,
    DEFINE,
    UNDEFINE,
    LOCK,
    UNLOCK,
    EMPTY,
    FULL
};


std::string stateToString(STATE s);

#endif
