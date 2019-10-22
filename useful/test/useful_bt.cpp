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
    Clock r = f+g;
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
    f+=Clock("04:04");
    EXPECT_EQ(f.getString(),"18:01");
}

TEST(ClockClass, _operator)
{
    EXPECT_FALSE(Clock("04:04") == Clock("04:05"));
    EXPECT_FALSE(Clock("05:05") == Clock("04:05"));
    EXPECT_TRUE(Clock("05:05") == Clock("05:05"));
    EXPECT_FALSE(Clock("05:05") != Clock("05:05"));
    EXPECT_TRUE(Clock("04:05") != Clock("05:05"));
    EXPECT_TRUE(Clock("04:05") != Clock("04:04"));
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
    nlohmann::json test_JSON = useful_F_libs::getJson("http://cyniu88.no-ip.pl/test/json/on_lightning.json");
    auto testKey = test_JSON["success"].get<bool>();

    std::cout << " JSON JEST" << std::endl << test_JSON.dump(4) << std::endl;

    EXPECT_TRUE(testKey);
}

//TEST(mkfifo_test, mkfifoFile)
//{
//    std::string path = "/mnt/ramdisk/FifoFile";
//    std::string msg = "p";
//    std::string returnString = "NULL";
//    int temp = mkfifo(path.c_str(),0666);

//    if ( temp == -1)
//        std::cout << "plik istnieje "<<strerror(errno)<< std::endl;
//    else if (temp == 0)
//        std::cout << "plik stworzony"<< std::endl;
//    else
//        FAIL();

//    useful_F_libs::write_to_mkfifo(path,msg);
//}

//TEST(mkfifo_test, mkfifoFile2)
//{
//    std::string msg = "p";
//    std::string path = "/mnt/ramdisk/FifoFile";
//    std::string returnString = useful_F_libs::read_from_mkfifo(path);
//    //unlink(path.c_str());

//    EXPECT_STREQ(returnString.c_str(), msg.c_str()) << "odczytano smieci";
//    //TODO not

//}
