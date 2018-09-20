#ifndef EVENT_COMMAND_H
#define EVENT_COMMAND_H
#include "../event_counters.h"

class event_command : public event_counters
{
public:
    event_command(const std::string &name);
    std::string help();
};
#endif // EVENT_COMMAND_H
