#include <iostream>
#include <string>

struct Clock{
    unsigned int h;
    unsigned int min;
    Clock () {}
    Clock(int h, int m):h(h),min(m){}
    void set(int h, int m){
        if (h<24 && m <60){
        this->h = h;
        this->min = m;
        }
        else {
            throw 0;
        }
    };
    std::string getString(){
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
};
