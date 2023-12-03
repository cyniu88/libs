#pragma once

#include "../event_counters.h"

class new_Connect_Event final  : public event_counters
{
public:
    explicit new_Connect_Event(const std::string &name);
    ~new_Connect_Event() = default;
    std::string help() const override;
};

