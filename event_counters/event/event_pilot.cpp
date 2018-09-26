#include "event_pilot.h"

event_pilot::event_pilot(const std::string& name): event_counters(name)
{

}

std::string event_pilot::help()
{
    return "pilot events";
}
