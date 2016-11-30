#ifndef EVENT_COUNTERS_HANDLER_H
#define EVENT_COUNTERS_HANDLER_H

#include "event_counters.h"

class event_counters_handler
{
    std::map <std::string, std::shared_ptr<event_counters> > eventCountersMap;
public:
    event_counters_handler();
    std::shared_ptr <event_counters> run(std::string name);
    std::string getListPossibleEvents();
    std::string help(std::string name = "");
};

#endif // EVENT_COUNTERS_HANDLER_H
