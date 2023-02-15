#include "viber_api.h"

size_t viber_API::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

viber_API::viber_API()
{

}

viber_API::viber_API(const std::string &accessToken, const std::string &url, const std::string &avatar): m_accessToken(accessToken),
    m_url(url),
    m_avatar(avatar)
{

}

void viber_API::setAccessToken(const std::string &accessToken)
{
    m_accessToken = accessToken;
}

void viber_API::setURL(const std::string &url)
{
    m_url = url;
}

void viber_API::setAvatar(const std::string &avatar)
{
    m_avatar = avatar;
}

std::string viber_API::sendViberMSG(const std::string &msg, const std::string &receiver, const std::string &senderName, const std::string &accessToken, const std::string &url)
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
    std::string data = "{ \"auth_token\":\"";
    data += token;
    data += "\",\"receiver\":\"";
    data += receiver;
    data += "\",\"min_api_version\":1,\"sender\":{\"name\":\"";
    data += senderName;
    data += "\",\"avatar\":\"";
    data += m_avatar;
    data += "\" },\"tracking_data\":\"tracking data\",\"type\":\"text\",\"text\":\"";
    data += msg;
    data += "\"  }";
    std::string address = Url;
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

std::string viber_API::sendViberPicture(const std::string &msg,
                                        const std::string &image,
                                        const std::string &receiver,
                                        const std::string &senderName,
                                        const std::string &accessToken,
                                        const std::string &url)
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
    std::string data = "{ \"auth_token\":\"";
    data += token;
    data += "\",\"receiver\":\"";
    data += receiver;
    data += "\",\"min_api_version\":1,\"sender\":{\"name\":\"";
    data += senderName;
    data += "\",\"avatar\":\"";
    data += m_avatar;
    data += "\" },\"tracking_data\":\"tracking data\",\"type\":\"picture\",\"text\":\"";
    data += msg;
    data += "\",\"media\":\"";
    data += image;
    data += "\"}";
    std::string address = Url;
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

std::string viber_API::sendViberUrl(const std::string &msg,
                                    const std::string &url2,
                                    const std::string &receiver,
                                    const std::string &senderName,
                                    const std::string &accessToken,
                                    const std::string &url)
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
    std::string data = "{ \"auth_token\":\"";
    data += token;
    data += "\",\"receiver\":\"";
    data += receiver;
    data += "\",\"min_api_version\":1,\"sender\":{\"name\":\"";
    data += senderName;
    data += "\",\"avatar\":\"";
    data += m_avatar;
    data += "\" },\"tracking_data\":\"tracking data\",\"type\":\"url\",\"text\":\"";
    data += msg;
    data += "\",\"media\":\"";
    data += url2;
    data += "\"}";
    std::string address = Url;
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