#include <curl/curl.h>
#include <string>

class FACEBOOK_API{
    std::string m_accessToken ;
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

public:
    FACEBOOK_API(std::string accessToken);
    FACEBOOK_API();
    std::string postTxtOnWall(std::string msg, std::string accessToken = "NULL");
    std::string postPhotoOnWall(std::string url,std::string msg = "",std::string accessToken = "NULL");
    void setAccessToken(std::string token);
};
