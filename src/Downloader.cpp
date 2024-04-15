#include "Downloader.h"

Downloader::Downloader() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
}

Downloader::~Downloader() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

std::vector<unsigned char> Downloader::download(const std::string& url, const std::vector<std::string>& proxy_list) {
    for (const auto& proxy : proxy_list) {
        setupCurlOptions(url, proxy);
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            break;
        }
    }
    return binaryData;
}

size_t Downloader::WriteCallback(void *buffer, size_t size, size_t nmemb, void *userp) {
    auto data = static_cast<std::vector<unsigned char>*>(userp);
    size_t totalSize = size * nmemb;
    data->insert(data->end(), (unsigned char*)buffer, (unsigned char*)buffer + totalSize);
    return totalSize;
}

void Downloader::setupCurlOptions(const std::string& url, const std::string& proxy) {
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: */*");
    headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");


    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &binaryData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.45 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate, br");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);\
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
}