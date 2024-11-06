#pragma once

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <optional>

#ifndef ANDROID
#include "json.hpp"
#endif

std::vector<std::string> split_string(const std::string& s, char separator );

class useful_F_libs {
public:

    ///////////////////// string ////////////////////////////////////////////////
    static void toLower(std::string& str);
    static bool hasSubstring(const std::string& _str, const std::string& _substring);
    static void write_to_mkfifo(const std::string& path, const std::string &msg);
    static std::string read_from_mkfifo(const std::string &path);
    static size_t  WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static std::string find_tag (const std::string &temp);
    static std::string stringToHex(const std::string& input);
    //////////////////// HTTP req //////////////////////////
    static std::string httpPost(const std::string &url, int timeoutSeconds);
    static std::string httpPost(const std::string &url);
    static void downloadFile(const std::string &url, const std::string &path, int timeoutSeconds);
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    static std::string removeHtmlTag(std::string &data);
    static std::optional<std::string> ipCountry(std::string &ip);
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
    std::stringstream os;
    os << value;
    return os.str();
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
    explicit Clock(const std::string& t){
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
    std::string getString() const{
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
    bool operator == (const Clock & c) const {
        if ((this->m_h == c.m_h) && (this->m_min == c.m_min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const Clock & c) const {
        if ((this->m_h != c.m_h) || (this->m_min != c.m_min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    friend std::ostream & operator<< (std::ostream &w , const Clock &c) {
        return w << c.getString();
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator < (const Clock& c) const {
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
    bool operator > (const Clock& c) const {
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
    bool operator >= (const Clock& c) const {
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
    bool operator <= (const Clock& c) const {
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
            minutes =  minutes % 60;
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
            minutes =  minutes % 60;
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

    unsigned int toSeconds() {
        return toSeconds(Clock(this->m_h, this->m_min) );
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static unsigned int toSeconds(Clock t) {
        return ((t.m_h*60) + t.m_min)*60;
    }
    /////////////////////////////////////////////////////////////////////////////////////

    static Clock fromSeconds(unsigned int sec) {
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
        return Clock::fromSeconds(end.toSeconds() + ( Clock::toSeconds(Clock(23,59))+ 60 - start.toSeconds() ) );
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static unsigned int getUnixTime()
    {
        return static_cast<unsigned int> (std::time(nullptr));
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static uint64_t getTimestamp()
    {
        return duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    ////////////////////////////////////////////////////////////////////////////////////
    static std::string unixTimeToString(unsigned int t)
    {
        std::time_t result = static_cast<std::time_t>(t);

        std::stringstream g;
        g << std::asctime(std::localtime(&result));

        return g.str();
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
#ifdef BT_TEST
        return Clock(m_BT_H,m_BT_M);
#else
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return Clock( static_cast <unsigned int>(ltm->tm_hour),static_cast <unsigned int>(ltm->tm_min) );
#endif
    }
    /////////////////////////////////////////////////////////////////////////////////////
    void stopwatchStart()
    {
        m_time = std::time(nullptr);
    }
    /////////////////////////////////////////////////////////////////////////////////////
    unsigned int  stopwatchStopAndGetResult() const 
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
    SEND_NOK,
    TEMPORARY,
    ENABLED,
    DISABLED,
    CONNECTED,
    DISCONNECTED,
    ARMED,
    DISARMED
    //WARNING remember add new state to stateToString() usefull.cpp
};

std::string stateToString(STATE s);
STATE stringToState(const std::string& s);

std::ostream &operator<<(std::ostream &os, const STATE &v);
std::ostream &operator>>(std::ostream &os,  STATE &v);

