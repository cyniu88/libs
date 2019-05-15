#include "generator.h"
#include <sstream>

generator::generator()
{

}

std::string generator::random_string(size_t length)
{
    const std::vector<char> char_array =
        {'0','1','2','3','4',
         '5','6','7','8','9',
         'A','B','C','D','E','F',
         'G','H','I','J','K',
         'L','M','N','O','P',
         'Q','R','S','T','U',
         'V','W','X','Y','Z',
         'a','b','c','d','e','f',
         'g','h','i','j','k',
         'l','m','n','o','p',
         'q','r','s','t','u',
         'v','w','x','y','z'
        };
    //1) create a non-deterministic random number generator
    std::default_random_engine rng(std::random_device{}());

    //2) create a random number "shaper" that will give
    //   us uniformly distributed indices into the character set
    std::uniform_int_distribution<> dist(0, char_array.size()-1);

    //3) create a function that ties them together, to get:
    //   a non-deterministic uniform distribution from the
    //   character set of your choice.
    auto randchar = [ char_array,&dist,&rng ](){return char_array[ dist(rng) ];};

    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

unsigned char generator::random_char() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return static_cast<unsigned char>(dis(gen));
}
std::string generator::generate_hex(size_t len)
{
    std::stringstream ss;
    for(size_t i = 0; i < len; i++) {
        auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << int(rc);
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}
