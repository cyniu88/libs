#include <sstream>
#include "emoji.h"

EMOJI::EMOJI()
{

}

std::string EMOJI::emoji(E_emoji e)
{
    switch (e) {
    case E_emoji::NORTH_EAST_ARROW:
        return HEX_STR::hexToStr("E28697");
    case E_emoji::SOUTH_EAST_ARROW:
        return HEX_STR::hexToStr("E28698");
    case E_emoji::NORTH_WEST_ARROW:
        return HEX_STR::hexToStr("E28696");
    case E_emoji::SOUTH_WEST_ARROW:
        return HEX_STR::hexToStr("E28699");
    case E_emoji::LEFT_RIGHT_ARROW:
        return HEX_STR::hexToStr("E28694");
    case E_emoji::UP_DOWN_ARROW:
        return HEX_STR::hexToStr("E28695");
    case E_emoji::ALARM_CLOCK:
        return HEX_STR::hexToStr("E28FB0");
    case E_emoji::SUN_WITH_FACE:
        return HEX_STR::hexToStr("F09F8C9E");
    case E_emoji::SNOWFLAKE:
        return HEX_STR::hexToStr("E29D84");
    case E_emoji::HEAVY_BLACK_HEART:
        return HEX_STR::hexToStr("E29DA4");
    case E_emoji::WARNING_SIGN:
        return HEX_STR::hexToStr("E29AA0");
    case E_emoji::CHART_WITH_DOWNWARDS_TREND:
        return HEX_STR::hexToStr("F09F9389");
    case E_emoji::CHART_WITH_UPWARDS_TREND:
        return HEX_STR::hexToStr("F09F9388");

    }
    return"ok";
}

std::string HEX_STR::hexToStr(std::string hex)
{
    std::string res;
    res.reserve(hex.size() / 2);
    for (unsigned int i = 0; i < hex.size(); i += 2)
    {
        std::istringstream iss(hex.substr(i, 2));
        int temp;
        iss >> std::hex >> temp;
        res += static_cast<char>(temp);
    }
    return res;
}
