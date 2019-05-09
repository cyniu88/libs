#include "alarm.h"
#include <sstream>

alarm_C::alarm_C()
{

}

void alarm_C::raiseAlarm(int id, const std::string &txt)
{
    std::lock_guard <std::mutex> lock (mutex_alarm);
    m_alarmMap[id] = txt;
}

void alarm_C::clearAlarm(int id)
{
    std::lock_guard <std::mutex> lock (mutex_alarm);
    m_alarmMap.erase(id);
}

std::string alarm_C::showAlarm()
{
    std::lock_guard <std::mutex> lock (mutex_alarm);
    std::stringstream ret;

    if(m_alarmMap.size() == 0)
        ret << " no alarms!";
    else{
        for (auto s: m_alarmMap) {
            ret << s.first << ": " << s.second << std::endl;
        }
    }
    return ret.str();
}

std::size_t alarm_C::alarmSize()
{
    std::lock_guard <std::mutex> lock (mutex_alarm);
    return m_alarmMap.size();
}
