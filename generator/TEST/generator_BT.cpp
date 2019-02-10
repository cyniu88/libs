#include <gtest/gtest.h>
#include "test_data.h"
#include "../generator.h"

class generator_BT: public ::testing::Test
{
public:
    void SetUp()
    {

    }
    void TearDown()
    {

    }
};

TEST_F(generator_BT, string256)
{
    int size = 256;
    generator g;
    auto str = g.random_string();
    EXPECT_EQ(str.size(), size);
    std::cout << str << std::endl;
}

TEST_F(generator_BT, hex)
{
    int size = 256;
    generator g;
    auto str = g.generate_hex(size);
    EXPECT_EQ(str.size(), size*2);
    std::cout << str << std::endl;
}
