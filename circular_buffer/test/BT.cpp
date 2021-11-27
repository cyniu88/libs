#include <gtest/gtest.h>

#include "../circular_buffer.h"
///////////////////////////////////////////////////// TEST ///////////////////////////////////////////////////////

TEST(Circural_buffer_bt, main)
{
    Circular_buffer testBuffer;
    for (char i = 40; i < 65; ++i){
        testBuffer.put(i);
    }
    std::cout << testBuffer.dump<char>();


}
