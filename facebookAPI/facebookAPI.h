#include <curl/curl.h>
#include <string>

class FACEBOOK_API{
    std::string m_accessToken;
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
    FACEBOOK_API(const std::string& accessToken);
    FACEBOOK_API();
    std::string postTxtOnWall(const std::string& msg,
                              const std::string& accessToken = "NULL");
    std::string postPhotoOnWall(const std::string& url,
                                const std::string& msg = "",
                                const std::string& accessToken = "NULL");
    void setAccessToken(const std::string& token);
};
