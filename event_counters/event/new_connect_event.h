#ifndef NEW_CONNECT_EVENT_H
#define NEW_CONNECT_EVENT_H

#include "../event_counters.h"

class new_Connect_Event final  : public event_counters
{
public:
    explicit new_Connect_Event(const std::string &name);
   ~new_Connect_Event( );
    std::string help() override;
};

#endif // NEW_CONNECT_EVENT_H
