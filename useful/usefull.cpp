#include "useful.h"

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
    case STATE::ERROR:      return "ERROR";
    default:
        return "UNKNOWN";
    }
}
