#include "viber_api.h"

size_t viber_API::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

viber_API::viber_API()
{

}

viber_API::viber_API(std::string accessToken, std::string url): m_accessToken(accessToken),m_url(url)
{

}

void viber_API::setAccessToken(std::string accessToken)
{
    m_accessToken = accessToken;
}

void viber_API::setURL(std::string url)
{
    m_url = url;
}

std::string viber_API::sendViberMSG(std::string msg, std::string user, std::string accessToken, std::string url)
{
    std::string token = m_accessToken;
    std::string Url = m_url;
    if (accessToken != "NULL"){
        token = accessToken;
    }
    if (url != "NULL"){
        Url = url;
    }

    CURL *curl;
        CURLcode res;
        std::string readBuffer;
        std::string data = "caption="+msg;
        data += "&url="+url;
        std::string address = Url+token;
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
