#include <iostream>

#include "event_counters/event_counters_handler.h"


int main(int argc, char *argv[])
{
    event_counters_handler mainEvent;

    std::cout << "Hello World!" << std::endl;

    mainEvent.run("connections")->addEvent("dupa");
    mainEvent.run("connections")->addEvent();

    mainEvent.run("co")->addEvent("16:59, 14-12-2013");

    mainEvent.run("unknown")->addEvent("kkskskks");
    int k = 100;
    for ( auto i =0; i< k; ++i){
        mainEvent.run("connections")->addEvent("iter: "+ std::to_string(i)+" awaria");
    }
    std::cout << "\n--------------------------------------------------------" << std::endl;
    std::cout << "mozliwe:\n"<< mainEvent.getListPossibleEvents() << std::endl;
    std::cout << "help:\n"<< mainEvent.help() << std::endl;
    std::cout << "help: dla connection\n"<< mainEvent.help("connections") << std::endl;
    std::cout << "\n--------------------------------------------------------" << std::endl;

    std::cout << "event: " <<mainEvent.run("connections")->howManyEvent()  << std::endl;
    std::cout << "event: " <<mainEvent.run("connections")->getEventName()  << std::endl;
    std::cout << "get event:\n" <<mainEvent.run("connections")->getEvent()  << std::endl;
    std::cout << "help : " <<mainEvent.run("connections")->help()  << std::endl;

    mainEvent.run("connections")->clearEvent();
    std::cout << "event: " <<mainEvent.run("connections")->howManyEvent()  << std::endl;
    std::cout << "get event:\n" <<mainEvent.run("connections")->getEvent()  << std::endl;

    std::cout << "event: " <<mainEvent.run("unknown")->howManyEvent()  << std::endl;
    return 0;
}
