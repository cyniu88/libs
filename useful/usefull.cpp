#include "useful.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

#ifndef IDOM
#include <curl/curl.h>
#endif
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
    case STATE::LOCK:       return "LOCK";
    case STATE::UNLOCK:     return "UNLOCK";
    case STATE::EMPTY:      return "EMPTY";
    case STATE::FULL:       return "FULL";
    case STATE::SEND_OK:    return "SEND_OK";
    case STATE::SEND_NOK:   return "SEND_NOK";
    case STATE::ENABLED:    return "ENABLED";
    case STATE::DISABLED:   return "DISABLED";
    default:
        return "UNKNOWN";
    }
}

STATE stringToState(const std::string& s){
    if(s == "OFF")
        return STATE::OFF;
    else if (s == "ON")
        return STATE::ON;
    else if (s == "PLAY")
        return STATE::PLAY;
    else if (s == "PAUSE")
        return STATE::PAUSE;
    else if (s == "STOP")
        return STATE::STOP;
    else if (s == "ACTIVE")
        return STATE::ACTIVE;
    else if (s == "DEACTIVE")
        return STATE::DEACTIVE;
    else if (s == "WORKING")
        return STATE::WORKING;
    else if (s == "DEFINE")
        return STATE::DEFINE;
    else if (s == "UNDEFINE")
        return STATE::UNDEFINE;
    else if (s == "LOCK")
        return STATE::LOCK;
    else if (s == "UNLOCK")
        return STATE::UNLOCK;
    else if (s == "EMPTY")
        return STATE::EMPTY;
    else if (s == "FULL")
        return STATE::FULL;
    else if (s == "SEND_OK")
        return STATE::SEND_OK;
    else if (s == "SEND_NOK")
        return STATE::SEND_NOK;
    else if (s == "ENABLED")
        return STATE::ENABLED;
    else if (s == "DISABLED")
        return STATE::DISABLED;
    else
        return STATE::UNKNOWN;
}

bool useful_F_libs::hasSubstring(const std::string& _str, const std::string& _substring)
{
    if(_str.find(_substring) != std::string::npos)
        return true;
    return false;
}

#ifndef IDOM

void useful_F_libs::write_to_mkfifo(const std::string &path, const std::string& msg)
{
    errno = 0;
    int fd = open(path.c_str(), O_RDWR| O_NONBLOCK );
    std::cout << "write open file: " << fd << " path " << path.c_str() << " msg: " << msg <<std::endl;
    std::cout << "write_to_mkfifo( error - " << strerror(  errno ) <<   std::endl;
    write(fd, msg.c_str(), msg.size());
    close(fd);
}

std::string useful_F_libs::read_from_mkfifo(const std::string& path)
{
    /* char buf[10];
    //open, read, and display the message from the FIFO
    int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    std::cout <<"read open file: " << fd <<std::endl;
    read(fd, buf, sizeof (buf));
    std::cout << "buf: " << buf << std::endl;
    close(fd);
    return (std::string(buf));
*/    std::string buf = "NULL";
    std::fstream fd;
    fd.open(path.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

    std::getline(fd,buf);
    return buf;
}

size_t useful_F_libs::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string useful_F_libs::find_tag(const std::string& temp)
{
    std::string value = "";
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

#ifndef BT_TEST
std::string useful_F_libs::httpPost(const std::string& url, int timeoutSeconds)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, useful_F_libs::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return readBuffer;
}

std::string useful_F_libs::httpPost(const std::string& url)
{
    return useful_F_libs::httpPost(url, 10);
}
#endif

void useful_F_libs::downloadFile(const std::string& url, const std::string& path, int timeoutSeconds)
{
    CURL *curl;
    //CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        FILE *fp = fopen(path.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        //res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        fclose(fp);
    }
}

std::string useful_F_libs::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
std::string useful_F_libs::removeHtmlTag(std::string &data)
{
    data = useful_F_libs::replaceAll(data,"</dl>","\n");

    //data = useful_F::replaceAll(data,"    "," ");
    bool copy = true;
    std::string plainString = "";
    std::stringstream convertStream;

    // remove all xml tags
    for (unsigned int i=0; i < data.length(); i++)
    {
        convertStream << data[i];

        if(convertStream.str().compare("<") == 0) copy = false;
        else if(convertStream.str().compare(">") == 0)
        {
            copy = true;
            convertStream.str(std::string());
            continue;
        }

        if(copy) plainString.append(convertStream.str());

        convertStream.str(std::string());
    }

    return plainString;
}

nlohmann::json useful_F_libs::getJson(const std::string& url)
{
    std::string str = useful_F_libs::httpPost(url);
    nlohmann::json jj = nlohmann::json::parse( str);

    return jj;
}
#endif
