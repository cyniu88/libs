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

std::vector<std::string> split_string(const std::string& s, char separator );
//#ifdef Q_OS_ANDROID
namespace std {
template <typename T>
int stoi(T s){
    return atoi(s.c_str());
}

template <typename T>
std::string to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
}
//#endif
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
        std::string ret;
        if (h<10) {
            ret+="0";
        }
        ret+=std::to_string(h);
        ret+=":";
        if (min<10) {
            ret+="0";
        }
        ret+=std::to_string(min);
        return ret;
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

enum class STATE {
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
    UNDEFINE
};


std::string stateToString(STATE s);

#endif
