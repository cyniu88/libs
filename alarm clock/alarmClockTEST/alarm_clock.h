#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H
#include <map>
#include <iostream>
#include <vector>
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
    std::vector<std::string> split( const std::string& stringToSplit, const std::string& delimiters) {
       using namespace std;
       // Skip all the text until the first delimiter is found
       string::size_type start = stringToSplit.find_first_not_of(delimiters, 0);
       string::size_type stop = stringToSplit.find_first_of(delimiters, start);

       std::vector<std::string> tokens;
       while (string::npos != stop || string::npos != start) {
          tokens.push_back(stringToSplit.substr(start, stop - start));
          start = stringToSplit.find_first_not_of(delimiters, stop);
          stop = stringToSplit.find_first_of(delimiters, start);
       }
       return tokens;
 }
};
struct Alarm {
    std::map < std::string ,Clock > alarmMap;
};

class Alarm_clock
{
    std::map <std::string, Alarm> alarmMap;
    Clock dummyForSplitString;
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
