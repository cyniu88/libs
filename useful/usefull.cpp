#include "useful.h"

#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <iostream>

#ifndef IDOM
#include <curl/curl.h>
#endif

void useful_F_libs::toLower(std::string &str)
{
    std::transform(str.cbegin(), str.cend(), str.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
}
std::vector<std::string> split_string(const std::string &s, char separator)
{
    std::vector<std::string> output;
    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(separator, pos)) not_eq std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        output.push_back(substring);
        prev_pos = ++pos;
    }
    try
    {
        output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    }
    catch (...)
    {
    }
    return output;
}

std::string stateToString(STATE s)
{
    switch (s)
    {
    case STATE::OFF:
        return "OFF";
    case STATE::ON:
        return "ON";
    case STATE::PLAY:
        return "PLAY";
    case STATE::PAUSE:
        return "PAUSE";
    case STATE::STOP:
        return "STOP";
    case STATE::ACTIVE:
        return "ACTIVE";
    case STATE::DEACTIVE:
        return "DEACTIVE";
    case STATE::WORKING:
        return "WORKING";
    case STATE::DEFINE:
        return "DEFINE";
    case STATE::UNDEFINE:
        return "UNDEFINE";
    case STATE::LOCK:
        return "LOCK";
    case STATE::UNLOCK:
        return "UNLOCK";
    case STATE::EMPTY:
        return "EMPTY";
    case STATE::FULL:
        return "FULL";
    case STATE::SEND_OK:
        return "SEND_OK";
    case STATE::SEND_NOK:
        return "SEND_NOK";
    case STATE::ENABLED:
        return "ENABLED";
    case STATE::DISABLED:
        return "DISABLED";
    case STATE::CONNECTED:
        return "CONNECTED";
    case STATE::DISCONNECTED:
        return "DISCONNECTED";
    case STATE::ARMED:
        return "ARMED";
    case STATE::DISARMED:
        return "DISARMED";
    default:
        return "UNKNOWN";
    }
}

STATE stringToState(const std::string &s)
{
    if (s == "OFF")
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
    else if (s == "CONNECTED")
        return STATE::CONNECTED;
    else if (s == "DISCONNECTED")
        return STATE::DISCONNECTED;
    else if (s == "ARMED")
        return STATE::ARMED;
    else if (s == "DISARMED")
        return STATE::DISARMED;
    else if (s == "null")
        return STATE::UNDEFINE;
    else
        return STATE::UNKNOWN;
}

std::ostream &operator<<(std::ostream &os, const STATE &v)
{
    os << stateToString(v);
    return os;
}

std::ostream &operator>>(std::ostream &os, STATE &v)
{
    std::stringstream ss;
    ss << os.rdbuf();
    v = stringToState(ss.str());
    return os;
}

bool useful_F_libs::hasSubstring(const std::string &_str, const std::string &_substring)
{
    if (_str.find(_substring) not_eq std::string::npos)
        return true;
    return false;
}

#ifndef IDOM

void useful_F_libs::write_to_mkfifo(const std::string &path, const std::string &msg)
{
    errno = 0;
    int fd = open(path.c_str(), O_RDWR | O_NONBLOCK);
    // std::cout << "write open file: " << fd << " path " << path.c_str() << " msg: " << msg <<std::endl;
    // std::cout << "write_to_mkfifo( error - " << strerror(  errno ) <<   std::endl;
    write(fd, msg.c_str(), msg.size());
    close(fd);
}

std::string useful_F_libs::read_from_mkfifo(const std::string &path)
{
    /* char buf[10];
    //open, read, and display the message from the FIFO
    int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    std::cout <<"read open file: " << fd <<std::endl;
    read(fd, buf, sizeof (buf));
    std::cout << "buf: " << buf << std::endl;
    close(fd);
    return (std::string(buf));
*/ std::string buf = "NULL";
    std::fstream fd;
    fd.open(path.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

    std::getline(fd, buf);
    return buf;
}

size_t useful_F_libs::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string useful_F_libs::find_tag(const std::string &temp)
{
    std::string value = "";
    for (unsigned int i = 0; i < temp.size(); ++i)
    {

        if (temp.at(i) == '>')
        {
            unsigned int z = i + 1;
            while (temp.at(z) not_eq '<')
            {
                value.push_back(temp.at(z));
                ++z;
            }
            break;
        }
    }
    return value;
}

std::string useful_F_libs::stringToHex(const std::string &input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

#ifndef BT_TEST
std::string useful_F_libs::httpPost(const std::string &url, int timeoutSeconds)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, useful_F_libs::WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res not_eq CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return readBuffer;
}

std::string useful_F_libs::httpPost(const std::string &url)
{
    return useful_F_libs::httpPost(url, 10);
}
#endif

void useful_F_libs::downloadFile(const std::string &url, const std::string &path, int timeoutSeconds)
{
    CURL *curl;
    // CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        FILE *fp = fopen(path.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeoutSeconds);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        // res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        fclose(fp);
    }
}

std::string useful_F_libs::replaceAll(std::string str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) not_eq std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
std::string useful_F_libs::removeHtmlTag(std::string &data)
{
    data = useful_F_libs::replaceAll(data, "</dl>", "\n");

    // data = useful_F::replaceAll(data,"    "," ");
    bool copy = true;
    std::string plainString = "";
    std::stringstream convertStream;

    // remove all xml tags
    for (unsigned int i = 0; i < data.length(); i++)
    {
        convertStream << data[i];

        if (convertStream.str().compare("<") == 0)
            copy = false;
        else if (convertStream.str().compare(">") == 0)
        {
            copy = true;
            convertStream.str(std::string());
            continue;
        }

        if (copy)
            plainString.append(convertStream.str());

        convertStream.str(std::string());
    }

    return plainString;
}

std::optional<std::string> useful_F_libs::ipCountry(std::string &ip)
{
    std::optional<std::string> country;
    try
    {
        nlohmann::json jj = nlohmann::json::parse(useful_F_libs::httpPost("http://ip-api.com/json/" + ip));

        if (jj.contains("country"))
            country = jj["country"];
    }
    catch (const std::exception &e)
    {
        std::cout << " błąd jsona w funkcji  " << __PRETTY_FUNCTION__ << std::endl;
    }

    return country;
}

long useful_F_libs::getMemoryUsageInMB()
{
    std::string path_v1 = "/sys/fs/cgroup/memory/memory.usage_in_bytes";
    std::string path_v2 = "/sys/fs/cgroup/memory.current";
    std::ifstream file;
    long memoryUsageBytes = 0;

    // Try cgroups v1 path
    file.open(path_v1);
    if (!file.is_open())
    {
        // If v1 path is not found, try cgroups v2 path
        file.open(path_v2);
    }

    if (file.is_open())
    {
        file >> memoryUsageBytes;
        file.close();
    }
    else
    {
        return -1;
    }

    return memoryUsageBytes / (1024 * 1024); // Convert bytes to megabytes
}

double useful_F_libs::getCpuUsage()
{
    std::string path_v1 = "/sys/fs/cgroup/cpu/cpuacct.usage";
    std::string path_v2 = "/sys/fs/cgroup/cpu.stat";
    std::ifstream file;
    long initialCpuUsage = 0;
    long finalCpuUsage = 0;

    // Function to read CPU usage in nanoseconds
    auto readCpuUsage = [&]() -> long
    {
        long cpuUsageNanoseconds = 0;

        // Try cgroups v1 path
        file.open(path_v1);
        if (file.is_open())
        {
            file >> cpuUsageNanoseconds;
            file.close();
        }
        else
        {
            // Try cgroups v2 path if v1 path is not available
            file.open(path_v2);
            if (file.is_open())
            {
                std::string line;
                while (std::getline(file, line))
                {
                    if (line.find("usage_usec") != std::string::npos)
                    {
                        cpuUsageNanoseconds = std::stol(line.substr(line.find(" ") + 1)) * 1000; // Convert microseconds to nanoseconds
                        break;
                    }
                }
                file.close();
            }
            else
            {
                return -1;
            }
        }

        return cpuUsageNanoseconds;
    };

    // Get initial CPU usage
    initialCpuUsage = readCpuUsage();
    if (initialCpuUsage == -1)
        return -1;

    // Wait a short period to measure CPU usage change over time
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Get CPU usage after waiting
    finalCpuUsage = readCpuUsage();
    if (finalCpuUsage == -1)
        return -1;

    // Calculate CPU usage difference over 1 second interval
    long cpuUsageDelta = finalCpuUsage - initialCpuUsage;
    double cpuUsagePercentage = (cpuUsageDelta / 1e9) * 100; // Convert nanoseconds to seconds and calculate percentage

    return cpuUsagePercentage;
}

nlohmann::json useful_F_libs::getJson(const std::string &url)
{
    std::string str = useful_F_libs::httpPost(url);
    return nlohmann::json::parse(str);
}
#endif
