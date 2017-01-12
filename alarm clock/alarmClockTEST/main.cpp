//#include <iostream>
//#include "alarm_clock.h"

//using namespace std;

//int main(int argc, char *argv[])
//{
//    cout << "Hello World!" << endl;
//    Alarm_clock myAlarm;

//    myAlarm.listAlarm();
//    std::cout << "start set alarm \n --------------" <<std::endl;
//    myAlarm.setAlarm("budzik","MON-THU-WED-THU-FRI-SAT-SUN",12,45);
//    myAlarm.setAlarm("budzik","SUN",12,30);
//    myAlarm.setAlarm("budzik","SUN-WEN-MON",2,5);
//    myAlarm.setAlarm("cyniu","SUN",8,00);
//    std::cout << "start list \n --------------" <<std::endl;
//    myAlarm.listAlarm();
//    std::cout << " \n --------------" <<std::endl;
//    myAlarm.removeAlarm("cyniu");
//    myAlarm.listAlarm();


//    return 0;
//}
#include <iostream>
#include <chrono>
#include <ctime>

long fibonacci(unsigned n)
{   std::cout << n;
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    std::cout << "f(142) = " << fibonacci(142) << '\n';
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
