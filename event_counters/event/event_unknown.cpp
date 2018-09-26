#include "event_unknown.h"

event_unknown::event_unknown(const std::string& name) : event_counters(name), name(name)
{

}

std::string event_unknown::help()
{
    return  name+" event";
}
