#include <gtest/gtest.h>
#include "test_data.h"
#include "../generator.h"

TEST(generator_BT, string256)
{
    size_t sizeTest = 256;
    generator g;
    auto str = g.random_string(sizeTest);
    auto str2 = g.random_string(sizeTest);
    EXPECT_EQ(str.size(), sizeTest);
    EXPECT_STRNE(str.c_str(),str2.c_str());
    std::cout << str << std::endl;
}

TEST(generator_BT, string10)
{
    size_t sizeTest = 10;
    generator g;
    auto str = g.random_string(sizeTest);
    auto str2 = g.random_string(sizeTest);
    EXPECT_EQ(str.size(), sizeTest);
    EXPECT_STRNE(str.c_str(),str2.c_str());
    std::cout << str << std::endl;
}

TEST(generator_BT, hex)
{
    size_t sizeTest = 255;
    generator g;
    auto str = g.generate_hex(sizeTest);
    EXPECT_EQ(str.size(), sizeTest*2);
    std::cout << str << std::endl;
}
