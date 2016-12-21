#include "alarm_clock.h"
#include <iostream>
Alarm_clock::Alarm_clock()
{

}

void Alarm_clock::setAlarm(std::string alarmName, std::string day, int h, int m)
{
    alarmMap[alarmName].alarmMap[day];
    auto a  =alarmMap[alarmName].alarmMap.find(day);
    a->second.set(h,m);
}

void Alarm_clock::addAlarm(std::string alarmName)
{
    alarmMap[alarmName];
    std::cout << "dodano alarm " << std::endl;
}

void Alarm_clock::removeAlarm(std::string alarmName)
{
    alarmMap.erase(alarmName);
    std::cout << "skasowano alarm " << std::endl;
}

void Alarm_clock::listAlarm()
{
    std::cout << "\n"<<std::endl;

    for (auto a : alarmMap){

        for (auto b : alarmMap[a.first].alarmMap){
            std::cout <<"alarm: "<<a.first <<" set: " <<b.first << " hour: "<< b.second.hour << " minutes: " << b.second.min <<std::endl;
        }
    }

}
