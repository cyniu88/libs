#ifndef NEW_CONNECT_EVENT_H
#define NEW_CONNECT_EVENT_H

#include "../event_counters.h"

class new_Connect_Event : public event_counters
{

public:
    new_Connect_Event(std::string name);
   ~new_Connect_Event( );
    std::string help();


};

#endif // NEW_CONNECT_EVENT_H
