#include <iostream>
#include "ProxyManager.h"



ProxyManager::ProxyManager(const std::string& filename) {
    loadProxies(filename);
}

bool ProxyManager::isValidProxy(const std::string& proxy) {
    Downloader request;
    //std::vector<std::string> proxy_list_unverif = {proxy};
    std::vector<unsigned char> response = request.download("https://www.etsy.com/", {proxy});
    return !response.empty();
}

std::vector<std::string> ProxyManager::CreateValidProxies() {
    for (const auto& proxy : proxies) {
        if (isValidProxy(proxy)) {
            validProxies.push_back(proxy);
        }
    }
    return validProxies;
}

void ProxyManager::loadProxies(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            proxies.push_back(line);
        }
        file.close();
    } else {
        std::cout << "Unable to open file: " << filename << std::endl;
    }
}

