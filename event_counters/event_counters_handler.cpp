#include "event_counters_handler.h"
#include "event/new_connect_event.h"
#include "event/event_unknown.h"
#include "event/event_mpd.h"
#include "event/event_pilot.h"
#include "event/event_command.h"


event_counters_handler::event_counters_handler()
{
    std::shared_ptr <event_counters> newConnectEvent (new new_Connect_Event("connections") );
    eventCountersMap.insert( std::make_pair( newConnectEvent->getEventName(), newConnectEvent ) );

    std::shared_ptr <event_counters> newUnknownEvent (new event_unknown("unknown") );
    eventCountersMap.insert( std::make_pair( newUnknownEvent->getEventName(), newUnknownEvent ) );

    std::shared_ptr <event_counters> eventMPD (new event_mpd("mpd") );
    eventCountersMap.insert( std::make_pair( eventMPD->getEventName(), eventMPD ) );

    std::shared_ptr <event_counters> eventPilot (new event_pilot("pilot") );
    eventCountersMap.insert( std::make_pair( eventPilot->getEventName(), eventPilot ) );

    std::shared_ptr <event_counters> eventCommand (new event_command("command") );
    eventCountersMap.insert( std::make_pair( eventCommand->getEventName(), eventCommand ) );
}

std::shared_ptr<event_counters> event_counters_handler::run(const std::string& name)
{

    if (eventCountersMap.find(name) == eventCountersMap.end()){
        addEvent(name);
    }
    return eventCountersMap[name];
}

std::string event_counters_handler::getListPossibleEvents()
{
    std::string result;

    for( auto iter= eventCountersMap.begin();iter != eventCountersMap.end(); ++iter ) {
        result.append(iter->first);
        result.push_back('\n');
    }
    return result;
}

std::string event_counters_handler::help(const std::string& name)
{
    std::string result;

    if (name.empty()){
        for( auto iter = eventCountersMap.begin();iter != eventCountersMap.end(); ++iter ) {
            result.append(iter->second->help());
            result.append("\n------------------------------\n");
        }
    }
    else{
        result = eventCountersMap[name]->help();
    }

    return result;
}
std::mutex event_counters_handler::echMutex;

void event_counters_handler::addEvent(const std::string& name)
{
    std::lock_guard <std::mutex> lock(event_counters_handler::echMutex);
    std::shared_ptr <event_counters> newUnknownEvent (new event_unknown(name) );
    eventCountersMap.insert( std::make_pair( newUnknownEvent->getEventName(), newUnknownEvent ) );
}

void event_counters_handler::clearOld(int moreThan, int last, std::function<void (std::string& )> logger)

{
    for( auto iter = eventCountersMap.begin();iter != eventCountersMap.end(); ++iter ) {

        auto i = iter->second->howManyEvent();
        if(i > moreThan){
            iter->second->clearEvent(0, i - last ) ;
            auto name = iter->second->getEventName();
            logger(name);
        }
    }
}

