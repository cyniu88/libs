#ifndef EVENT_UNKNOWN_H
#define EVENT_UNKNOWN_H

#include "../event_counters.h"

class event_unknown : public event_counters
{
    std::string  name;
public:
    event_unknown(std::string name);
    std::string help();
};

#endif // EVENT_UNKNOWN_H
