#ifndef EVENT_MPD_H
#define EVENT_MPD_H

#include "../event_counters.h"

class event_mpd : public event_counters
{
public:
    event_mpd(std::string name);
    std::string help();
};

#endif // EVENT_MPD_H
