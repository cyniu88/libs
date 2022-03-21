#include "new_connect_event.h"
#include <iostream>

new_Connect_Event::new_Connect_Event(const std::string& name) : event_counters(name)
{

}

new_Connect_Event::~new_Connect_Event()
{
    std::cout << " kasujemy obiekt" << std::endl;
}

std::string new_Connect_Event::help() const
{
    return "connection start event";
}






