#ifndef EVENT_PILOT_H
#define EVENT_PILOT_H


#include "../event_counters.h"

class event_pilot final : public event_counters
{
public:
    explicit event_pilot(const std::string &name);
    std::string help() const override;
};

#endif // EVENT_PILOT_H
