#ifndef VIBER_API_H
#define VIBER_API_H

#include <curl/curl.h>
#include <string>

class viber_API
{
    std::string m_accessToken;
    std::string m_url;

    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
    viber_API();
    viber_API(std::string accessToken, std::string url);
    void setAccessToken(std::string accessToken);
    void setURL(std::string url);
    std::string sendViberMSG(std::string msg, std::string user ,std::string accessToken = "NULL",std::string url = "NULL");
};

#endif // VIBER_API_H
