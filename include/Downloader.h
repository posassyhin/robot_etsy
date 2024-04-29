#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <curl/curl.h>
#include <vector>
#include <string>
#include <iostream>

/**
 * @brief Класс для загрузки данных из интернета.
 */
class Downloader {
public:
    /**
     * @brief Конструктор класса Downloader.
     */
    Downloader();

    /**
     * @brief Деструктор класса Downloader.
     */
    ~Downloader();
    
    /**
     * @brief Загружает данные по указанному URL с использованием списка прокси.
     * 
     * @param url URL-адрес, по которому происходит загрузка.
     * @param proxy_list Список прокси-серверов для загрузки данных.
     * @return std::vector<unsigned char> Вектор с загруженными данными.
     */
    std::vector<unsigned char> download(const std::string& url, const std::vector<std::string>& proxy_list);

private:
    CURL* curl; ///< Указатель на CURL сессию.
    std::vector<unsigned char> binaryData; ///< Вектор для хранения загруженных данных.

    /**
     * @brief Callback-функция для записи получаемых данных.
     * 
     * @param buffer Указатель на буфер с данными.
     * @param size Размер одного элемента данных.
     * @param nmemb Количество элементов данных.
     * @param userp Пользовательский указатель (не используется).
     * @return size_t Количество успешно записанных данных.
     */
    static size_t WriteCallback(void *buffer, size_t size, size_t nmemb, void *userp);

    /**
     * @brief Настроить опции CURL для сессии.
     * 
     * @param url URL-адрес для загрузки.
     * @param proxy Прокси-сервер, используемый для загрузки.
     */
    void setupCurlOptions(const std::string& url, const std::string& proxy);
};

#endif // DOWNLOADER_H