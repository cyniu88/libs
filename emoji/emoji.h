#pragma once

#include <string.h>
#include <iostream>

enum class E_emoji{
    NORTH_EAST_ARROW = 1,
    SOUTH_EAST_ARROW = 2,
    LEFT_RIGHT_ARROW,
    UP_DOWN_ARROW,
    NORTH_WEST_ARROW,
    SOUTH_WEST_ARROW,
    ALARM_CLOCK,
    SUN_WITH_FACE,
    SNOWFLAKE,
    HEAVY_BLACK_HEART,
    WARNING_SIGN,
    CHART_WITH_UPWARDS_TREND,
    CHART_WITH_DOWNWARDS_TREND,
    THUNDER_CLOUD_AND_RAIN,
    THERMOMETER,
    BULB
     ///// do not forget add to class EMOJI emoji
};
class HEX_STR
{
public:
    static std::string hexToStr(const std::string &hex);
};
class EMOJI
{
public:
    EMOJI();
    static std::string emoji(E_emoji e);
};
