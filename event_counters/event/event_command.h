#ifndef EVENT_COMMAND_H
#define EVENT_COMMAND_H
#include "../event_counters.h"

class event_command final : public event_counters
{
public:
    explicit event_command(const std::string &name);
    std::string help() override;
};
#endif // EVENT_COMMAND_H
