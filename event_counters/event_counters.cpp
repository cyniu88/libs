#include "event_counters.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <algorithm>

event_counters::event_counters(std::string name) : eventName(std::move(name))
{

}

int event_counters::howManyEvent()
{
    std::lock_guard < std::mutex > lock ( eventMutex);
    return static_cast<int>( eventList.size() );
}

void event_counters::addEvent(const std::string& note)
{
    eventStruct d;
    std::ostringstream oss;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");

    d.date = oss.str();
    d.note = note;
    d.posixTime = static_cast<unsigned int> (std::time(nullptr));
    std::lock_guard < std::mutex > lock ( eventMutex);
    eventList.push_back(d);
}

std::string event_counters::getEvent()
{
    std::stringstream ret;
    ret << "Event: " << help() << "\n";
    std::lock_guard < std::mutex > lock ( eventMutex);
    int k =0;
    for (auto i : eventList){
        ret << ++k << "\t" << i.date << "     " <<  i.note << "\n";
    }
    return ret.str();
}

void event_counters::clearEvent()
{
    std::lock_guard < std::mutex > lock ( eventMutex);
    eventList.clear();
}

void event_counters::clearEvent(unsigned int from, unsigned int to)
{
    if (to < from){
        return;
    }
    auto max = static_cast<unsigned int>( eventList.size() );

    if (max < to){
        to = max;
    }
    if(max<from){
        from = max;
        to = max;
    }
    std::lock_guard < std::mutex > lock ( eventMutex);
    eventList.erase(eventList.begin()+from, eventList.begin()+to);
}

unsigned int event_counters::getLast1minNumberEvent()
{
    int k = 0;
    unsigned int lastPosix = eventList.at(eventList.size()-1).posixTime;
    std::lock_guard < std::mutex > lock ( eventMutex);

    for (auto i = eventList.size()-1; i != -1; i--){
        if(eventList.at(i).posixTime+60 > lastPosix)
            k++;
        else
            break;

    }
    return k;
}

std::string event_counters::getEventName()
{
    return eventName;
}
