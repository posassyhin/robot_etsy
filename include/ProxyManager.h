#ifndef PROXY_MANAGER_H
#define PROXY_MANAGER_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Downloader.h" // Используемый класс Downloader должен быть объявлен в своем header файле

class ProxyManager {
public:
    explicit ProxyManager(const std::string& filename);

    std::vector<std::string> validProxies;

    bool isValidProxy(const std::string& proxy);
    std::vector<std::string> CreateValidProxies();

private:
    std::vector<std::string> proxies;

    void loadProxies(const std::string& filename);
};

#endif // PROXY_MANAGER_H