#ifndef VIBER_API_H
#define VIBER_API_H

#include <curl/curl.h>
#include <string>

class viber_API
{
    std::string m_accessToken;
    std::string m_url;
    std::string m_avatar;
    std::string m_name;
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
    viber_API();
    viber_API(const std::string& accessToken, const std::string& url, const std::string& avatar);
    void setAccessToken(const std::string& accessToken);
    void setURL(const std::string& url);
    void setAvatar (const std::string& avatar);
    std::string sendViberMSG(const std::string& msg,
                             const std::string& receiver,
                             const std::string& senderName,
                             const std::string& accessToken = "NULL",
                             const std::string& url = "NULL");
    std::string sendViberPicture(const std::string& msg,
                                 const std::string& image,
                                 const std::string& receiver,
                                 const std::string& senderName,
                                 const std::string& accessToken = "NULL",
                                 const std::string& url = "NULL");
    std::string sendViberUrl(const std::string& msg,
                             const std::string& url2,
                             const std::string& receiver,
                             const std::string& senderName,
                             const std::string& accessToken = "NULL",
                             const std::string& url = "NULL");
};

#endif // VIBER_API_H
