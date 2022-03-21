#include "event_command.h"

event_command::event_command(const std::string& name):  event_counters(name)
{

}

std::string event_command::help() const
{
    return "event command";
}
