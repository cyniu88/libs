#include <iostream>
#include "../useful.h"


int main()
{
    std::cout << "Hello World!" << std::endl;
    Clock c = Clock::getTime();

    Clock k( 0,1);
    std::cout << "godzina "<< c.getString()<< std::endl;
    if (c < k) {
        std::cout << "c < k ! " <<std::endl;
    }
    if (c > k) {
        std::cout << "c > k ! " <<std::endl;
    }
    if (c == k) {
        std::cout << "c == k ! " <<std::endl;
    }
    if (c >= k) {
        std::cout << "c >= k ! " <<std::endl;
    }
    if (c <= k) {
        std::cout << "c <= k ! " <<std::endl;
    }
    std::cout << " c " << c <<  " k " << k << std::endl;


         c+=k;


    std::cout << " dodawanie " << c << std::endl;
    return 0;
}
