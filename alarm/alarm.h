#ifndef ALARM_H
#define ALARM_H

#include <map>
#include <string>
#include <mutex>

class alarm_C
{
    std::map <int,std::string> m_alarmMap;
    std::mutex mutex_alarm;
public:
    alarm_C();
    void raiseAlarm(int id, const std::string& txt);
    void clearAlarm(int id);
    std::string showAlarm();
    std::size_t alarmSize();
};

#endif // ALARM_H
