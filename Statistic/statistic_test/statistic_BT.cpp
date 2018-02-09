#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../statistic.h"

TEST(StatisticClass, average)
{
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(1);
    average.push_back(2);
    average.push_back(2);
    average.push_back(3);
    average.push_back(3);
    average.push_back(4);
    average.push_back(4);
    average.push_back(-5);
    average.push_back(-5);
    average.push_back(6);
    average.push_back(6);

    EXPECT_DOUBLE_EQ(average.average() , 1.8333333333333333);
}

TEST(StatisticClass, median)
{
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(1);
    average.push_back(2);
    average.push_back(2);
    average.push_back(3);
    average.push_back(3);
    average.push_back(4);
    average.push_back(4);
    average.push_back(-5);
    average.push_back(-5);
    average.push_back(6);
    average.push_back(6);

    EXPECT_EQ(average.median(),2.5);
}

TEST(StatisticClass, min)
{
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(1);
    average.push_back(2);
    average.push_back(2);
    average.push_back(3);
    average.push_back(3);
    average.push_back(4);
    average.push_back(4);
    average.push_back(-5);
    average.push_back(-5);
    average.push_back(6);
    average.push_back(6);

    EXPECT_EQ(average.min(),-5);
}
TEST(StatisticClass, max)
{
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(1);
    average.push_back(2);
    average.push_back(2);
    average.push_back(3);
    average.push_back(3);
    average.push_back(4);
    average.push_back(4);
    average.push_back(-5);
    average.push_back(-5);
    average.push_back(6);
    average.push_back(6);

    EXPECT_EQ(average.max(),6);
}

TEST(StatisticClass, resize)
{
    unsigned int s = 3;
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(2);
    average.push_back(3);
    average.push_back(4);
    average.push_back(5);
    average.push_front(44);
    average.push_front(45);
    average.print();
    average.resize(s);

    EXPECT_EQ(average.size(),static_cast<double>(s));

    auto data = average.getLast2();
    average.print();

    EXPECT_EQ(data.first,4);
    EXPECT_EQ(data.second,5);
}

TEST(StatisticClass, moreDiff)
{
    STATISTIC<double> average(12);
    average.push_back(1);
    average.push_back(2);
    average.push_back(30);
    average.push_back(4);
    average.push_back(5);
    average.push_front(44);
    average.push_front(45);
    average.print();

    EXPECT_FALSE(average.isMoreDiff(2.2));

    average.push_back(7.3);
    average.print();

    EXPECT_TRUE(average.isMoreDiff(2.2));
}
