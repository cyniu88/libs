#include <iostream>
#include "../sunriseset.h"


int main(){

    std::cout << " ======= START ======"<<std::endl;
    SunRiseSet mySunRiseSet;


    mySunRiseSet.printAllData();
    std::cout << " ======== STOP ======" << std::endl;

    std::cout << "DAY: " << mySunRiseSet.getDay() << std::endl;
    std::cout << "MOUNTH: " << mySunRiseSet.getMounth() << std::endl;
    std::cout << "YEAR: " << mySunRiseSet.getYear() << std::endl;

    Clock k;
    k = mySunRiseSet.getSunSet();
    std::cout << "SunSet: " <<k.getString() << std::endl;
    k = mySunRiseSet.getSunRise();
    std::cout << "SunRise: " <<k.getString() << std::endl;
    k = mySunRiseSet.getDayLength();
    std::cout << "Day Lenght: " <<k.getString() << std::endl;
    return 0;
}

