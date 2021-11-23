#include <gtest/gtest.h>

#include "../circular_buffer.h"
///////////////////////////////////////////////////// TEST ///////////////////////////////////////////////////////

TEST(Circural_buffer_bt, main)
{
    Circular_buffer testBuffer;
    for (int i = 1; i < 25; ++i){
        testBuffer.put(i);
    }
    std::cout << testBuffer.dump();
}
