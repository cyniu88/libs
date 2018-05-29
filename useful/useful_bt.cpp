#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "useful.h"
///////////////////////////////////////////////////// TEST ///////////////////////////////////////////////////////\

TEST(ClockClass, AddTwoHours)
{
    Clock f(13,57);
    Clock g(23,59);
    Clock r = f+g;
    EXPECT_EQ(r.getString(), "13:56");
}

TEST(ClockClass, lessThen_Hours)
{
    Clock f(13,57);
    Clock g(23,59);
    EXPECT_EQ(f<g, true);
    //EXPECT_LT(f,g);
}

TEST(ClockClass, plus_operator)
{
    Clock f(13,57);
    f+=Clock("04:04");
    EXPECT_EQ(f.getString(),"18:01");
}

TEST(ClockClass, periodOfTime)
{
    Clock f(13,57);
    Clock g(22,22);
    Clock r = Clock::periodOfTime(f,g);
    EXPECT_EQ(r.getString(),"08:25");
}

TEST(ClockClass, from_to_second)
{
    Clock f;
    f.set(13,57);
    unsigned int sec = f.toSeconds();
    Clock g = Clock::fromSeconds(sec);
    ASSERT_EQ(f.getString(),g.getString());
}

TEST(JSON, getJSON)
{
    nlohmann::json test_JSON = useful_F_libs::getJson("http://api.gios.gov.pl/pjp-api/rest/data/getData/401");
    std::string testKey = test_JSON["key"].get<std::string>();

    std::cout << " JSON JEST"<<std::endl << test_JSON.dump(4) << std::endl;

    ASSERT_STREQ("PM10",testKey.c_str());
}
