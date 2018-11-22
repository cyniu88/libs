#ifndef EVENT_COUNTERS_H
#define EVENT_COUNTERS_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <mutex>

struct eventStruct{
    std::string date = "";
    std::string note = "";
    unsigned int posixTime = 0;
};

class event_counters
{
    std::mutex eventMutex;

protected:
#ifdef BT_TEST
public:
#endif
    std::string eventName;
    std::vector <eventStruct> eventList;

public:
    event_counters(std::string name);
    virtual std::string help() = 0;
    virtual int howManyEvent();
    virtual void addEvent(const std::string &note = "");
    virtual std::string getEvent ();
    virtual void clearEvent();
    virtual void clearEvent(unsigned int from, unsigned int to);
    virtual unsigned int getLast1minNumberEvent();
    std::string getEventName();
private:
    virtual unsigned int getLast1minNumberEvent_NO_Mutex();
};

#endif // EVENT_COUNTERS_H
