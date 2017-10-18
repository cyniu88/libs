#include <iostream>
#include <string>
#include <ostream>
#include <chrono>

struct Clock{
    unsigned int h;
    unsigned int min;
    Clock () {}
    Clock(int h, int m) {
        set(h,m);
    }
    void set(int h, int m){
        if (h<24 && m <60){
        this->h = h;
        this->min = m;
        }
        else {
            throw 0;
        }
    }
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
    bool operator ==( const Clock & c){
        if ((this->h == c.h) && (this->min == c.min)){
            return true;
        }
        else{
            return false;
        }
    }
    bool operator !=( const Clock & c){
        if ((this->h != c.h) || (this->min != c.min)){
            return true;
        }
        else{
            return false;
        }
    }

    friend std::ostream & operator<< (std::ostream &w ,  Clock &c) {

        return w   << c.getString();
     }

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
   static Clock getTime()
    {
        Clock t;
        time_t now = time(0);
        tm *ltm = localtime(&now);
        t.set( ltm->tm_hour ,ltm->tm_min );
        return t;
    }
};
