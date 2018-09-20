#include "facebookAPI.h"

size_t FACEBOOK_API::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

FACEBOOK_API::FACEBOOK_API(const std::string& accessToken): m_accessToken(accessToken)
{

}

FACEBOOK_API::FACEBOOK_API()
{

}

std::string FACEBOOK_API::postTxtOnWall(const std::string& msg, const std::string& accessToken)
{
    std::string token = m_accessToken;
    if (accessToken != "NULL"){
        token = accessToken;
    }
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string data = "message="+msg;
    std::string address = "https://graph.facebook.com/v2.10/me/feed?access_token="+token;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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

std::string FACEBOOK_API::postPhotoOnWall(const std::string &url, const std::string& msg, const std::string& accessToken)
{
    std::string token = m_accessToken;
    if (accessToken != "NULL"){
        token = accessToken;
    }
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    std::string data = "caption="+msg;
    data += "&url="+url;
    std::string address = "https://graph.facebook.com/v2.10/me/photos?access_token="+token;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, address.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
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

void FACEBOOK_API::setAccessToken(const std::string &token)
{
    m_accessToken = token;
}
