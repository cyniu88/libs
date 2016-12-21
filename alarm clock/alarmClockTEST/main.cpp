#include <iostream>
#include "alarm_clock.h"
using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    Alarm_clock myAlarm;

    myAlarm.listAlarm();
    std::cout << "start set alarm \n --------------" <<std::endl;
    myAlarm.setAlarm("budzik","MON",12,45);
    myAlarm.setAlarm("budzik","SUN",12,30);
    myAlarm.setAlarm("budzik","SUN",12,32);
    myAlarm.setAlarm("cyniu","SUN",8,00);
    std::cout << "start list \n --------------" <<std::endl;
    myAlarm.listAlarm();
    std::cout << " \n --------------" <<std::endl;
    myAlarm.removeAlarm("cyniu");
    myAlarm.listAlarm();
    return 0;
}
