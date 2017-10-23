#include <iostream>
#include "../facebookAPI.h"



int main(int argc, char *argv[])
{
    std::cout << "Hello World!" << std::endl;

    FACEBOOK_API fb("");
    fb.setAccessToken("");
    std::cout <<"post: "<< fb.postPhotoOnWall("https://prokompc.pl/wp-content/uploads/2015/01/usmiech.jpg","domek") << std::endl;
    return 0;
}
