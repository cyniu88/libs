#include "useful.h"

std::vector<std::string> split_string(const std::string& s, char separator ){
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while((pos = s.find(separator, pos)) != std::string::npos)
    {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        output.push_back(substring);
        prev_pos = ++pos;
    }
    try {
        output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
    }
    catch (...){

    }
    return output;
}

std::string stateToString(STATE s){
    switch (s) {
    case STATE::OFF:        return "OFF";
    case STATE::ON:         return "ON";
    case STATE::PLAY:       return "PLAY";
    case STATE::PAUSE:      return "PAUSE";
    case STATE::STOP:       return "STOP";
    case STATE::ACTIVE:     return "ACTIVE";
    case STATE::DEACTIVE:   return "DEACTIVE";
    case STATE::WORKING:    return "WORKING";
    case STATE::DEFINE:     return "DEFINE";
    case STATE::UNDEFINE:   return "UNDEFINE";
    case  STATE::LOCK:      return "LOCK";
    case  STATE::UNLOCK:    return "UNLOCK";
    case  STATE::EMPTY:     return "EMPTY";
    case  STATE::FULL:      return "FULL";
   // case STATE::ERROR:      return "ERROR";
    default:
        return "UNKNOWN";
    }
}

