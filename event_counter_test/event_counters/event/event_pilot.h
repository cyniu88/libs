#ifndef EVENT_PILOT_H
#define EVENT_PILOT_H


#include "../event_counters.h"

class event_pilot : public event_counters
{
public:
    event_pilot(const std::string &name);
    std::string help();
};

#endif // EVENT_PILOT_H
