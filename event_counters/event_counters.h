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
   mutable std::mutex eventMutex;

protected:
#ifdef BT_TEST
public:
#endif
    std::string eventName;
    std::vector <eventStruct> eventList;

public:
    event_counters(std::string name);
    virtual ~event_counters() = default;
    virtual std::string help() const = 0;
    virtual int howManyEvent() const;
    virtual void addEvent(const std::string &note = "");
    virtual std::string getEvent() const;
    virtual void clearEvent();
    virtual void clearEvent(int from, int to);
    virtual unsigned int getLast1minNumberEvent() const;
    std::string getEventName() const;
private:
    virtual unsigned int getLast1minNumberEvent_NO_Mutex() const;
};

#endif // EVENT_COUNTERS_H
