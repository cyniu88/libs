#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <vector>
#include <random>
#include <functional> //for std::function
#include <algorithm> //for std::generate_n

class generator
{
public:
generator();
// given a function that generates a random character,
// return a string of the requested length
std::string random_string( size_t length = 256 );
unsigned char random_char();
std::string generate_hex(const unsigned int len);

};

#endif // GENERATOR_H
