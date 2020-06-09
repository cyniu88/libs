#ifndef EVENT_UNKNOWN_H
#define EVENT_UNKNOWN_H

#include "../event_counters.h"

class event_unknown final : public event_counters
{
    std::string  name;
public:
    explicit event_unknown(const std::string &name);
    std::string help() override;
};

#endif // EVENT_UNKNOWN_H
