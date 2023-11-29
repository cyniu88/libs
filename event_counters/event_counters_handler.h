#pragma once

#include "event_counters.h"
#include <functional>
#include <map>
#include <memory>

class event_counters_handler
{
    std::map <std::string, std::shared_ptr<event_counters> > eventCountersMap;

public:
    static std::mutex echMutex;
    event_counters_handler();
    std::shared_ptr <event_counters> run(const std::string &name);
    std::string getListPossibleEvents();
    std::string help(const std::string &name = "");
    void addEvent(const std::string &name);
    void clearOld(int moreThan, int last, std::function<void(std::string& name )> logger);
};

