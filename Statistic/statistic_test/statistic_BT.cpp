#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../statistic.h"

class StatisticClass_fixture : public ::testing::Test
{
public:
    STATISTIC<double> average;
    StatisticClass_fixture(): average(12)
    {
    }
    void SetUp()
    {
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
    }
    void TearDown()
    {

    }
};

TEST_F(StatisticClass_fixture, range)
{
    EXPECT_DOUBLE_EQ(average.average() , 1.8333333333333333);
    EXPECT_EQ(average.range(), 11);
}

TEST_F(StatisticClass_fixture, average)
{
    EXPECT_DOUBLE_EQ(average.average() , 1.8333333333333333);
    std::string ret = average.stats();
    EXPECT_THAT(ret, testing::HasSubstr("max"));
}

TEST_F(StatisticClass_fixture, averageOne)
{
    average.resize(1);
    average.push_back(1);
    EXPECT_DOUBLE_EQ(average.average() , 1.0);
}

TEST_F(StatisticClass_fixture, averageZero)
{
    STATISTIC<double>a(1);
    std::cout << "Srednia: " << a.average() << std::endl;
    EXPECT_DOUBLE_EQ(a.average() , 0.0);
}

TEST_F(StatisticClass_fixture, medianZero)
{
    STATISTIC<double>a(1);
    std::cout << "Mediana: " << a.average() << std::endl;
    EXPECT_DOUBLE_EQ(a.average() , 0.0);
}

TEST_F(StatisticClass_fixture, median)
{
    EXPECT_EQ(average.median(),2.5f);
}

TEST_F(StatisticClass_fixture, trend)
{
    STATISTIC<int> data(200);
    for (int i = 200; i != 0; --i){
        puts("dupa");
        data.push_back(i);
    }
    std::cout << data.trend();
}

TEST(StatisticClass, medianOne)
{
    STATISTIC<double> average(1);
    average.push_back(1);
    EXPECT_EQ(average.median(), 1.0f);
}

TEST_F(StatisticClass_fixture, min)
{
    EXPECT_EQ(average.min(),-5.0f);
}

TEST_F(StatisticClass_fixture, max)
{
    EXPECT_EQ(average.max(),6);
}

TEST_F(StatisticClass_fixture, push_and_pop)
{
    average.pop_back();
    EXPECT_EQ(average.getSize(),11);
    average.push_front(3.9);
    EXPECT_EQ(average.getSize(),12);
    average.pop_back();
    EXPECT_EQ(average.getSize(),11);
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

    EXPECT_EQ(average.getSize(),static_cast<double>(s));

    auto data = average.getLast2();
    average.print();

    EXPECT_EQ(data.first,4.0);
    EXPECT_EQ(data.second,5.0);
}

TEST(StatisticClass, getLast2_empty)
{
    unsigned int s = 1;
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
    average.push_front(33);

    EXPECT_EQ(average.getSize(),static_cast<double>(s));

    auto data = average.getLast2();
    average.print();

    EXPECT_EQ(data.first,0.0);
    EXPECT_EQ(data.second,0.0);
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

TEST(StatisticClass, moreDiff_wrong)
{
    STATISTIC<double> average(1);
    average.push_back(1);

    average.print();

    EXPECT_FALSE(average.isMoreDiff(2.2));

    average.push_back(7.3);
    average.print();

    EXPECT_FALSE(average.isMoreDiff(2.2));
}

TEST(StatisticClass, noMoreDiff)
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

    EXPECT_FALSE(average.isMoreDiff(6.2));

    average.push_back(7.3);
    average.print();

    EXPECT_FALSE(average.isMoreDiff(9.2));
}

TEST(StatisticClass, mode)
{
    STATISTIC<double> average(22);
    average.push_back(29.62);
    average.push_back(29.71);
    average.push_back(30.19);
    average.push_back(33.5);
    average.push_back(30.31);
    average.push_back(30.81);
    average.push_back(30.87);
    average.push_back(31.25);
    average.push_back(31.31);
    average.push_back(31.56);
    average.push_back(31.87);
    average.push_back(31.94);
    average.push_back(32.13);
    average.push_back(32.13);  //to
    average.push_back(32.63);

    average.print();
    std::cout << "1 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(32.13,average.mode()) << "ZLA DOMINANTA 32.13";
    average.push_back(33.5);
    average.push_back(33.5);
    std::cout << "2 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(33.5,average.mode()) << "ZLA DOMINANTA 33.5";
}

TEST(StatisticClass, modeZero)
{
    STATISTIC<double> average(1);

    std::cout << "0 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(average.mode(), 0) ;
}

TEST(StatisticClass, modeOne)
{
    STATISTIC<double> average(1);
    average.push_back(29.62);
    average.print();
    std::cout << "1 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(29.62,average.mode()) << "ZLA DOMINANTA 32.13";
}

TEST(StatisticClass, modeTwo)
{
    STATISTIC<double> average(10);
    average.push_back(29.62);
    average.push_back(28.62);
    average.print();
    std::cout << "1 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(28.62,average.mode()) << "ZLA DOMINANTA 32.13";
}
TEST(StatisticClass, modeThree)
{
    STATISTIC<double> average(10);
    average.push_back(29.62);
    average.push_back(28.62);
    average.push_back(29.63);
    average.print();
    std::cout << "1 MODE: " << average.mode() << std::endl;
    EXPECT_DOUBLE_EQ(28.62,average.mode()) << "ZLA DOMINANTA 32.13";
}

TEST(StatisticClass, coefficientOfVariation)
{
    int i = 270;
    STATISTIC<int> testDB(i);

    for (int j = 0; j < 100; ++j)
    {
        testDB.push_back(77);
    }

    for (int j = 0; j < 100; ++j)
    {
        testDB.push_back(83);
    }
    for (int j = 0; j < 100; ++j)
    {
        testDB.push_back(95);
    }

    EXPECT_NE(testDB.coefficientOfVariation(),0);
}

TEST(StatisticClass, getPrintout)
{
    STATISTIC<double> data(10);

    EXPECT_NO_THROW(data.print());
}

TEST(StatisticClass, statsZero)
{
    STATISTIC<double>db(1);
    db.stats();

}
