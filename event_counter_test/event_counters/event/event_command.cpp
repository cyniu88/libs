#include "event_command.h"

event_command::event_command(std::string name):  event_counters(name)
{

}

std::string event_command::help()
{
    return "event command";
}
