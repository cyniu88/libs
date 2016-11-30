#ifndef EVENT_COUNTERS_H
#define EVENT_COUNTERS_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <mutex>

struct eventStruct{
    std::string date ="";
    std::string note ="";
};

class event_counters
{
std::mutex eventMutex;

protected:
    //int counter = 0;
    std::string eventName;
     std::vector <eventStruct> eventList;
public:
    event_counters(std::string name);
    virtual std::string help() = 0;
    virtual int howManyEvent() ;
    virtual void addEvent(std::string note = "");
    virtual std::string getEvent ();
    virtual void clearEvent();
    std::string getEventName();
};

#endif // EVENT_COUNTERS_H
