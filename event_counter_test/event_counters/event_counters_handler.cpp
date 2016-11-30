#include "event_counters_handler.h"
#include "event/new_connect_event.h"
#include "event/event_unknown.h"


event_counters_handler::event_counters_handler()
{
    std::shared_ptr <event_counters> newConnectEvent (new new_Connect_Event("connections") );
    eventCountersMap.insert(  std::make_pair( newConnectEvent->getEventName(),   newConnectEvent  )  );

    std::shared_ptr <event_counters> newUnknownEvent (new event_unknown("unknown") );
    eventCountersMap.insert(  std::make_pair( newUnknownEvent->getEventName(),   newUnknownEvent  )  );
}

std::shared_ptr<event_counters> event_counters_handler::run(std::string name)
{
   // return eventCountersMap[name];

    if (eventCountersMap.find(name) == eventCountersMap.end()){
        return eventCountersMap["unknown"];
    }
    else{
        return  eventCountersMap[name];
    }
}

std::string event_counters_handler::getListPossibleEvents()
{
    std::string result;

    for( auto  iter= eventCountersMap.begin();iter != eventCountersMap.end(); ++iter ) {

                result+= iter->first;
                result+= "\n";
    }

    return result;
}

std::string event_counters_handler::help(std::string name)
{
    std::string result;

    if (name == ""){
        for( auto  iter= eventCountersMap.begin();iter != eventCountersMap.end(); ++iter ) {
                    result+= iter->second->help();
                    result+= "\n------------------------------\n";
        }
    }
    else{
        result = eventCountersMap[name]->help();
    }

    return result;
}
