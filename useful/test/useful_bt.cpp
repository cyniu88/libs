#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../useful.h"
///////////////////////////////////////////////////// TEST ///////////////////////////////////////////////////////\

TEST(ClockClass, AddTwoHours)
{
    Clock f(13,57);
    Clock g(23,59);
    Clock r = f + g;
    EXPECT_EQ(r.getString(), "13:56");
}

TEST(ClockClass, lessThen_Hours)
{
    Clock f(13,57);
    Clock g(23,59);
    EXPECT_EQ(f<g, true);
}

TEST(ClockClass, plus_operator)
{
    Clock f(13,57);
    f += Clock("04:04");
    EXPECT_EQ(f.getString(),"18:01");
}

TEST(ClockClass, _operator)
{
    EXPECT_FALSE(Clock("04:04") == Clock("04:05"));
    EXPECT_FALSE(Clock("05:05") == Clock("04:05"));
    EXPECT_TRUE(Clock("05:05") == Clock("05:05"));
    EXPECT_FALSE(Clock("05:05") not_eq Clock("05:05"));
    EXPECT_TRUE(Clock("04:05") not_eq Clock("05:05"));
    EXPECT_TRUE(Clock("04:05") not_eq Clock("04:04"));
    EXPECT_TRUE(Clock("04:05") > Clock("04:04"));
    EXPECT_FALSE(Clock("03:05") > Clock("04:04"));
    EXPECT_TRUE(Clock("04:05") >= Clock("04:04"));
    EXPECT_FALSE(Clock("04:05") <= Clock("04:04"));
    EXPECT_FALSE(Clock("05:05") <= Clock("04:05"));
    EXPECT_TRUE(Clock("03:05") <= Clock("04:05"));
    EXPECT_TRUE(Clock("04:05") <= Clock("04:06"));
    EXPECT_FALSE(Clock("05:05") < Clock("04:06"));
    EXPECT_TRUE(Clock("04:05") < Clock("04:06"));
    EXPECT_TRUE(Clock("08:05") > Clock("04:06"));

    std::stringstream s;
    Clock f("00:00");
    s << f;
    EXPECT_STREQ("00:00", s.str().c_str());

    Clock g("20:00");
    g += Clock("23:43");
    EXPECT_STREQ("19:43", g.getString().c_str());
}

TEST(ClockClass, periodOfTime)
{
    Clock f(13,57);
    Clock g(22,22);
    Clock r = Clock::periodOfTime(f,g);
    EXPECT_EQ(r.getString(),"08:25");
    r = Clock::periodOfTime(g,f);
    EXPECT_EQ(r.getString(),"15:35");
}

TEST(ClockClass, from_to_second)
{
    Clock f;
    f.set(13,57);
    unsigned int sec = f.toSeconds();
    Clock g = Clock::fromSeconds(sec);
    EXPECT_EQ(f.getString(),g.getString());
}

TEST(ClockClass, stopwatch)
{
    Clock f;
    f.stopwatchStart();
    sleep(1);
    EXPECT_EQ(1,f.stopwatchStopAndGetResult());
}

TEST(ClockClass, wrongSet)
{
    Clock f;
    EXPECT_ANY_THROW(f.set(99,99));
}

TEST(ClockClass, to_string_with_precision_TC)
{
    double d = 1.0/3.0;
    EXPECT_STREQ("0.33", to_string_with_precision(d,2).c_str() );
}

TEST(JSON, getJSON)
{
    nlohmann::json test_JSON = useful_F_libs::getJson("http://45.90.3.84/test/json/on_lightning.json");
    auto testKey = test_JSON["success"].get<bool>();

    std::cout << " JSON JEST" << std::endl << test_JSON.dump(4) << std::endl;

    EXPECT_TRUE(testKey);
}

TEST(usefull_libs, gtoLower)
{
    std::string testU("teST");
    std::string testL("test");

    useful_F_libs::toLower(testU);

    EXPECT_STREQ(testU.c_str(), testL.c_str());
}

TEST(usefull_libs, ipCountry)
{
    std::string ip {"45.90.3.84"};
    std::string country{"Poland"};
    EXPECT_STREQ(country.c_str(), useful_F_libs::ipCountry(ip).value_or("dupa").c_str());
}