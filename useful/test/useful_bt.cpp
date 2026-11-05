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

// ──────────────────────────────────────────────────────────────────────────────
// stateToString / stringToState – pokrycie gałęzi enum (usefull.cpp ~53% linii)
// ──────────────────────────────────────────────────────────────────────────────
TEST(StateConversion, stateToString_all_values)
{
    EXPECT_EQ(stateToString(STATE::OFF),          "OFF");
    EXPECT_EQ(stateToString(STATE::ON),           "ON");
    EXPECT_EQ(stateToString(STATE::PLAY),         "PLAY");
    EXPECT_EQ(stateToString(STATE::PAUSE),        "PAUSE");
    EXPECT_EQ(stateToString(STATE::STOP),         "STOP");
    EXPECT_EQ(stateToString(STATE::ACTIVE),       "ACTIVE");
    EXPECT_EQ(stateToString(STATE::DEACTIVE),     "DEACTIVE");
    EXPECT_EQ(stateToString(STATE::WORKING),      "WORKING");
    EXPECT_EQ(stateToString(STATE::DEFINE),       "DEFINE");
    EXPECT_EQ(stateToString(STATE::UNDEFINE),     "UNDEFINE");
    EXPECT_EQ(stateToString(STATE::LOCK),         "LOCK");
    EXPECT_EQ(stateToString(STATE::UNLOCK),       "UNLOCK");
    EXPECT_EQ(stateToString(STATE::EMPTY),        "EMPTY");
    EXPECT_EQ(stateToString(STATE::FULL),         "FULL");
    EXPECT_EQ(stateToString(STATE::SEND_OK),      "SEND_OK");
    EXPECT_EQ(stateToString(STATE::SEND_NOK),     "SEND_NOK");
    EXPECT_EQ(stateToString(STATE::ENABLED),      "ENABLED");
    EXPECT_EQ(stateToString(STATE::DISABLED),     "DISABLED");
    EXPECT_EQ(stateToString(STATE::CONNECTED),    "CONNECTED");
    EXPECT_EQ(stateToString(STATE::DISCONNECTED), "DISCONNECTED");
    EXPECT_EQ(stateToString(STATE::ARMED),        "ARMED");
    EXPECT_EQ(stateToString(STATE::DISARMED),     "DISARMED");
    EXPECT_EQ(stateToString(STATE::UNKNOWN),      "UNKNOWN");
}

TEST(StateConversion, stringToState_all_values)
{
    EXPECT_EQ(stringToState("OFF"),          STATE::OFF);
    EXPECT_EQ(stringToState("ON"),           STATE::ON);
    EXPECT_EQ(stringToState("PLAY"),         STATE::PLAY);
    EXPECT_EQ(stringToState("PAUSE"),        STATE::PAUSE);
    EXPECT_EQ(stringToState("STOP"),         STATE::STOP);
    EXPECT_EQ(stringToState("ACTIVE"),       STATE::ACTIVE);
    EXPECT_EQ(stringToState("DEACTIVE"),     STATE::DEACTIVE);
    EXPECT_EQ(stringToState("WORKING"),      STATE::WORKING);
    EXPECT_EQ(stringToState("DEFINE"),       STATE::DEFINE);
    EXPECT_EQ(stringToState("UNDEFINE"),     STATE::UNDEFINE);
    EXPECT_EQ(stringToState("LOCK"),         STATE::LOCK);
    EXPECT_EQ(stringToState("UNLOCK"),       STATE::UNLOCK);
    EXPECT_EQ(stringToState("EMPTY"),        STATE::EMPTY);
    EXPECT_EQ(stringToState("FULL"),         STATE::FULL);
    EXPECT_EQ(stringToState("SEND_OK"),      STATE::SEND_OK);
    EXPECT_EQ(stringToState("SEND_NOK"),     STATE::SEND_NOK);
    EXPECT_EQ(stringToState("ENABLED"),      STATE::ENABLED);
    EXPECT_EQ(stringToState("DISABLED"),     STATE::DISABLED);
    EXPECT_EQ(stringToState("CONNECTED"),    STATE::CONNECTED);
    EXPECT_EQ(stringToState("DISCONNECTED"), STATE::DISCONNECTED);
    EXPECT_EQ(stringToState("ARMED"),        STATE::ARMED);
    EXPECT_EQ(stringToState("DISARMED"),     STATE::DISARMED);
    EXPECT_EQ(stringToState("null"),         STATE::UNDEFINE);
    EXPECT_EQ(stringToState("totally_unknown"), STATE::UNKNOWN);
}

TEST(StateConversion, roundtrip)
{
    // stateToString → stringToState musi dać oryginał
    for (auto s : {STATE::OFF, STATE::ON, STATE::LOCK, STATE::UNLOCK,
                   STATE::ARMED, STATE::DISARMED, STATE::CONNECTED, STATE::DISCONNECTED})
    {
        EXPECT_EQ(stringToState(stateToString(s)), s);
    }
}

// ──────────────────────────────────────────────────────────────────────────────
// useful_F_libs free functions
// ──────────────────────────────────────────────────────────────────────────────
TEST(UsefulLibs, replaceAll_basic)
{
    EXPECT_EQ(useful_F_libs::replaceAll("aa bb aa", "aa", "XX"), "XX bb XX");
    EXPECT_EQ(useful_F_libs::replaceAll("nothingtochange", "ZZ", "XX"), "nothingtochange");
    EXPECT_EQ(useful_F_libs::replaceAll("aaa", "a", "bb"), "bbbbbb");
}

TEST(UsefulLibs, stringToHex_basic)
{
    EXPECT_EQ(useful_F_libs::stringToHex("A"), "41");
    EXPECT_EQ(useful_F_libs::stringToHex(""), "");
    EXPECT_EQ(useful_F_libs::stringToHex(std::string(1, '\x00')), "00");
}

TEST(UsefulLibs, removeHtmlTag_strips_tags)
{
    std::string html = "<b>Hello</b> <i>World</i>";
    EXPECT_EQ(useful_F_libs::removeHtmlTag(html), "Hello World");
}

TEST(UsefulLibs, find_tag_extracts_content)
{
    EXPECT_EQ(useful_F_libs::find_tag("<tag>value</tag>"), "value");
    EXPECT_EQ(useful_F_libs::find_tag("<a>123</a>"), "123");
}

TEST(UsefulLibs, hasSubstring)
{
    EXPECT_TRUE(useful_F_libs::hasSubstring("hello world", "world"));
    EXPECT_FALSE(useful_F_libs::hasSubstring("hello world", "xyz"));
    EXPECT_TRUE(useful_F_libs::hasSubstring("abc", ""));
}

TEST(UsefulLibs, toLower)
{
    std::string s = "HeLLo WoRLd";
    useful_F_libs::toLower(s);
    EXPECT_EQ(s, "hello world");
}
