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
    static  void write_to_mkfifo(const std::string& path, const std::string &msg);
    static  std::string read_from_mkfifo(const std::string &path);
    static size_t  WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static std::string find_tag (const std::string &temp);
    //////////////////// HTTP req //////////////////////////
    static std::string httpPost(const std::string &url, int timeoutSeconds);
    static std::string httpPost(const std::string &url);
    static void downloadFile(const std::string &url, const std::string &path, int timeoutSeconds);
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    static std::string removeHtmlTag(std::string &data);
    /////////////////////  JSON ////////////////////////////

#ifndef ANDROID
    static nlohmann::json getJson(const std::string &url);
#endif
};
namespace std
{

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
} // namespace std

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 4)
{
    std::ostringstream out;
    out << std::setprecision(n) << a_value;
    return out.str();
}

struct Clock{
private:
    std::time_t m_time;
#ifdef BT_TEST
    static unsigned int m_BT_H;
    static unsigned int m_BT_M;
#endif
public:
    unsigned int m_h = 0;
    unsigned int m_min = 0;
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
            this->m_h = h;
            this->m_min = m;
        }
        else {
            throw 0;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    const std::string getString(){
        std::stringstream ret;
        if (m_h < 10) {
            ret << "0";
        }
        ret << m_h;
        ret << ":";
        if (m_min < 10) {
            ret << "0";
        }
        ret << m_min;
        return ret.str();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const Clock & c){
        if ((this->m_h == c.m_h) && (this->m_min == c.m_min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const Clock & c){
        if ((this->m_h != c.m_h) || (this->m_min != c.m_min)){
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
        if (this->m_h < c.m_h){
            return true;
        }
        else{
            if (this->m_h == c.m_h && this->m_min < c.m_min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator > (const Clock& c){
        if (this->m_h > c.m_h){
            return true;
        }
        else{
            if (this->m_h == c.m_h && this->m_min > c.m_min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator >= (const Clock& c){
        if (this->m_h > c.m_h){
            return true;
        }
        else if (this->m_h == c.m_h){

            if (this->m_min >= c.m_min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator <= (const Clock& c){
        if (this->m_h < c.m_h){
            return true;
        }
        else if (this->m_h == c.m_h){

            if (this->m_min <= c.m_min){
                return true;
            }
        }
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////
    Clock  operator + (const Clock& c){
        unsigned int minutes, hours;
        minutes = m_min+ c.m_min;
        hours = m_h + c.m_h;
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
        minutes = m_min+ c.m_min;
        hours = m_h + c.m_h;
        if (minutes >59){
            minutes =  minutes % 60 ;
            hours+=1;
        }
        if (hours >= 24){
            hours-=24;
        }
        this->m_h = hours;
        this->m_min = minutes;
        return *this;

    }
    /////////////////////////////////////////////////////////////////////////////////////

    unsigned int toSeconds(){
        return toSeconds(Clock(this->m_h, this->m_min) );
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static unsigned int toSeconds(Clock t){
        return ((t.m_h*60) + t.m_min)*60;
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
#ifdef BT_TEST
    static void setTime_forBT_usage(int h, int m)
    {
        m_BT_H = h;
        m_BT_M = m;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////
    static Clock getTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
#ifdef BT_TEST
        return Clock(m_BT_H,m_BT_M);
#else
        return Clock( static_cast <unsigned int>(ltm->tm_hour),static_cast <unsigned int>(ltm->tm_min) );
#endif
    }
    /////////////////////////////////////////////////////////////////////////////////////
    void stopwatchStart()
    {
        m_time = std::time(nullptr);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    unsigned int  stopwatchStopAndGetResult()
    {
        return static_cast<unsigned int>(std::time(nullptr) - m_time);
    }
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
    UNDEFINE,
    LOCK,
    UNLOCK,
    EMPTY,
    FULL,
    SEND_OK,
    SEND_NOK
    //WARNING remember add new state to stateToString() usefull.cpp
};


std::string stateToString(STATE s);
STATE stringToState(const std::string& s);

#endif
