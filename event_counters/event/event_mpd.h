#pragma once

#include "../event_counters.h"

class event_mpd final : public event_counters
{
public:
    explicit event_mpd(const std::string &name);
    std::string help() const override;
};

