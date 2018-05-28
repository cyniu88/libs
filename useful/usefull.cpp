#include "useful.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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


void useful_F_libs::write_to_mkfifo(std::string path,  std::string msg)
{
    int fd = open(path.c_str(), O_WRONLY| O_NONBLOCK);
    write(fd, msg.c_str(), msg.size());
    close(fd);
}

std::string useful_F_libs::read_from_mkfifo(std::string path)
{
    char buf[10];
    /* open, read, and display the message from the FIFO */
    int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    read(fd, buf, 10);
    close(fd);
    return (std::string(buf));
}

size_t useful_F_libs::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string useful_F_libs::find_tag(const std::string& temp)
{
    std::string value="";
    for (unsigned int i = 0; i<temp.size();++i){

        if (temp.at(i) =='>')
        {  int z = i+1;
            while (temp.at(z)!='<')
            {
                value+= temp.at(z);
                ++z;
            }
            break;
        }
    }
    return value;
}
