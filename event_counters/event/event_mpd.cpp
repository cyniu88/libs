#include "event_mpd.h"

event_mpd::event_mpd(const std::string& name): event_counters(name)
{

}

std::string event_mpd::help() const
{
    return "event from MPD";
}
