#include "new_connect_event.h"

new_Connect_Event::new_Connect_Event(const std::string& name) : event_counters(name)
{}

std::string new_Connect_Event::help() const
{
    return "connection start event";
}






