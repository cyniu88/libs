#include <gtest/gtest.h>
#include "../emoji.h"

// Pokrycie emoji.cpp – switch wszystkich wartości E_emoji + HEX_STR::hexToStr

TEST(EmojiTest, hexToStr_converts_correctly)
{
    // 'A' = 0x41
    EXPECT_EQ(HEX_STR::hexToStr("41"), "A");
    // "Hi" = 0x48 0x69
    EXPECT_EQ(HEX_STR::hexToStr("4869"), "Hi");
    EXPECT_EQ(HEX_STR::hexToStr(""), "");
}

TEST(EmojiTest, all_emoji_values_return_nonempty_string)
{
    EMOJI e;
    // każda gałąź switcha musi zwrócić niepusty string
    EXPECT_FALSE(e.emoji(E_emoji::NORTH_EAST_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::SOUTH_EAST_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::NORTH_WEST_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::SOUTH_WEST_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::LEFT_RIGHT_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::UP_DOWN_ARROW).empty());
    EXPECT_FALSE(e.emoji(E_emoji::ALARM_CLOCK).empty());
    EXPECT_FALSE(e.emoji(E_emoji::SUN_WITH_FACE).empty());
    EXPECT_FALSE(e.emoji(E_emoji::SNOWFLAKE).empty());
    EXPECT_FALSE(e.emoji(E_emoji::HEAVY_BLACK_HEART).empty());
    EXPECT_FALSE(e.emoji(E_emoji::WARNING_SIGN).empty());
    EXPECT_FALSE(e.emoji(E_emoji::CHART_WITH_DOWNWARDS_TREND).empty());
    EXPECT_FALSE(e.emoji(E_emoji::CHART_WITH_UPWARDS_TREND).empty());
    EXPECT_FALSE(e.emoji(E_emoji::THUNDER_CLOUD_AND_RAIN).empty());
    EXPECT_FALSE(e.emoji(E_emoji::THERMOMETER).empty());
    EXPECT_FALSE(e.emoji(E_emoji::BULB).empty());
}

TEST(EmojiTest, different_emojis_return_different_strings)
{
    EMOJI e;
    EXPECT_NE(e.emoji(E_emoji::SUN_WITH_FACE), e.emoji(E_emoji::SNOWFLAKE));
    EXPECT_NE(e.emoji(E_emoji::ALARM_CLOCK),   e.emoji(E_emoji::BULB));
}
