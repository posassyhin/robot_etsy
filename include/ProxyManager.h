#ifndef PROXY_MANAGER_H
#define PROXY_MANAGER_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "Downloader.h" // Включение определения класса Downloader

/**
 * @brief Класс для управления и проверки списка прокси-серверов.
 */
class ProxyManager {
public:
    /**
     * @brief Конструктор класса ProxyManager.
     * 
     * @param filename Имя файла, содержащего список прокси-серверов.
     */
    explicit ProxyManager(const std::string& filename);

    std::vector<std::string> validProxies; ///< Список проверенных и работающих прокси-серверов.

    /**
     * @brief Проверяет, является ли прокси-сервер действительным и работающим.
     * 
     * @param proxy Адрес прокси-сервера для проверки.
     * @return bool Возвращает true, если прокси действителен и работает.
     */
    bool isValidProxy(const std::string& proxy);

    /**
     * @brief Создаёт список действительных прокси-серверов.
     * 
     * @return std::vector<std::string> Список действительных прокси-серверов.
     */
    std::vector<std::string> CreateValidProxies();

private:
    std::vector<std::string> proxies; ///< Список всех прокси-серверов, загруженных из файла.

    /**
     * @brief Загружает список прокси-серверов из файла.
     * 
     * @param filename Имя файла, содержащего список прокси-серверов.
     */
    void loadProxies(const std::string& filename);
};

#endif // PROXY_MANAGER_H
