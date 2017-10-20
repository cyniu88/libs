#include <iostream>
#include <string>
#include <ostream>
#include <chrono>

struct Clock{
    unsigned int h = 0;
    unsigned int min = 0;
    Clock () {}
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
    bool operator ==( const Clock & c){
        if ((this->h == c.h) && (this->min == c.min)){
            return true;
        }
        else{
            return false;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////
    bool operator !=( const Clock & c){
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
    static Clock getTime()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return Clock( ltm->tm_hour ,ltm->tm_min );
    }
    /////////////////////////////////////////////////////////////////////////////////////
};
