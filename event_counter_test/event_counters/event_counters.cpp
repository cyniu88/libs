#include "event_counters.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>


event_counters::event_counters(std::string name) : eventName(name)
{

}

int event_counters::howManyEvent()
{
    std::lock_guard < std::mutex > lock ( eventMutex);
    return eventList.size();
}

void event_counters::addEvent(    std::string note)
{
    eventStruct d;
    std::ostringstream oss;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
//oss << "dupa";
    d.date = oss.str();
    d.note = note;
    std::lock_guard < std::mutex > lock ( eventMutex);
    eventList.push_back(d);
}

std::string event_counters::getEvent()
{
    std::string ret;
    std::lock_guard < std::mutex > lock ( eventMutex);
    for (auto i : eventList){
        ret += i.date + " "+ i.note + "\n";
    }

    return ret;
}

void event_counters::clearEvent( )
{
    std::lock_guard < std::mutex > lock ( eventMutex);
    eventList.clear();
}

std::string event_counters::getEventName()
{
    return eventName;
}
