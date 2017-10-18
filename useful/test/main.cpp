#include <iostream>
#include "../useful.h"


int main()
{
    std::cout << "Hello World!" << std::endl;
    Clock c(12,59);
    Clock k( 12,2);
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
    return 0;
}
