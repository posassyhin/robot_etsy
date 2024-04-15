#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <iostream>

class Downloader {
public:
    Downloader();
    ~Downloader();
    
    std::vector<unsigned char> download(const std::string& url, const std::vector<std::string>& proxy_list);

private:
    CURL* curl;
    std::vector<unsigned char> binaryData;

    static size_t WriteCallback(void *buffer, size_t size, size_t nmemb, void *userp);
    void setupCurlOptions(const std::string& url, const std::string& proxy);
};

#endif // DOWNLOADER_H