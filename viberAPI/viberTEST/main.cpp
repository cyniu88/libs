#include <iostream>
#include "../viber_api.h"

int main()
{
    viber_API viber(" "," "," ");
    std::cout << "Hello World!" << std::endl;

    std::cout << viber.sendViberMSG("  "," "," ") << std::endl;
    return 0;
}
