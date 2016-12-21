#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H
#include <map>
#include <iostream>
struct Day{
    bool MON = false;
    bool TUE = false;
    bool WED = false;
    bool THU = false;
    bool FRI = false;
    bool SAT = false;
    bool SUN = false;
};

struct Clock {
    bool isSet = false;
    int hour = 0;
    int min  = 0;
    void set(int h, int m){
        this->hour  = h;
        this->min   = m;
        this->isSet = true;
        std::cout << " ustawiono " << h <<":"<<m <<std::endl;
    }
};
struct Alarm {
    std::map < std::string ,Clock > alarmMap;
};

class Alarm_clock
{
    std::map <std::string, Alarm> alarmMap;
public:
    Alarm_clock();
    void checkAlarm();
    void seveAlarm();
    void readAlarm();
    void setAlarm(std::string alarmName, std::string day, int h, int m);
    void addAlarm(std::string alarmName);
    void removeAlarm(std::string alarmName);
    void listAlarm ();
};

#endif // ALARM_CLOCK_H
