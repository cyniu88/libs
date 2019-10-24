#ifndef EVENT_MPD_H
#define EVENT_MPD_H

#include "../event_counters.h"

class event_mpd final : public event_counters
{
public:
    explicit event_mpd(const std::string &name);
    std::string help() override;
};

#endif // EVENT_MPD_H
