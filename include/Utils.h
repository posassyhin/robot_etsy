#ifndef UTILS_H
#define UTILS_H

#include <gumbo.h>
#include <thread>
#include <chrono>
#include <regex>
#include <windows.h>
#include "ProxyManager.h"
#include "Downloader.h"
#include "HTMLParser.h"


class Utils {
    public:
        /**
         * @brief Преобразует вектор байтов в строку.
         * @param data Вектор байтов для конвертации.
         * @return Преобразованная строка.
         */
        static std::string convertVectorToString(const std::vector<unsigned char>& data) {
            std::string result(data.begin(), data.end());
            return result;
        }


        /**
         * @brief Считывает содержимое файла, интерпретируя каждую строку как отдельную ссылку.
         * @param filename Имя файла для чтения.
         * @return Вектор строк, каждая из которых является ссылкой.
         */
        static  std::vector<std::string> get_parsed_links(const std::string& filename){
            std::vector<std::string> parsed_links;
            std::ifstream file(filename);
            std::string line;

            if (!file.is_open()) {
                std::cerr << "Unable to open file: " << filename << std::endl;
                return parsed_links;
            }

            while (getline(file, line)) {
                parsed_links.push_back(line);
                //std::cout << "Pased links: " << line << std::endl;
            }
            file.close();

            return parsed_links;
        }

        /**
         * @brief Ищет подстроку в каждом элементе вектора строк.
         * @param input Вектор исходных строк.
         * @param substring Подстрока для поиска.
         * @return Вектор строк, содержащих подстроку.
         */
        static std::vector<std::string> find_substring_in_vector(const std::vector<std::string>& input, const std::string& substring) {
            std::vector<std::string> results;
            for (const auto& item : input) {
                if (item.find(substring) != std::string::npos) {
                    results.push_back(item);
                }
            }
            return results;
        }


        /**
         * @brief Заменяет размеры изображений в URL с помощью регулярного выражения.
         * @param url URL изображения для изменения.
         * @param newSize Новый размер в формате строки (например, "100x100").
         * @return Изменённый URL.
         */
        static std::string replace_image_size_with_regex(const std::string& url, const std::string& newSize) {
            std::regex sizeRegex(R"(\d+xN)");

            std::string modifiedUrl = std::regex_replace(url, sizeRegex, newSize);

            if (modifiedUrl == url) {
                return "";
            }

            return modifiedUrl;
        }


        /**
         * @brief Записывает байтовые данные в файл.
         * @param imageData Данные изображения для сохранения.
         * @param filepath Путь к файлу для сохранения.
         * @param append Флаг дозаписи данных в конец файла.
         * @return Успешность выполнения операции.
         */
        static bool save_to_file(const std::vector<unsigned char>& imageData, const std::string& filepath, bool append = false) {
            std::ios::openmode mode = std::ios::binary;
            if (append) {
                mode |= std::ios::app;
            } else {
                mode |= std::ios::trunc;
            }

            std::ofstream file(filepath, mode);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << filepath << std::endl;
                return false;
            }


            file.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
            file.write("\n", 1);

            file.close();

            return true;
        }


        /**
         * @brief Извлекает идентификатор продукта из URL.
         * @param url URL для анализа.
         * @return Идентификатор продукта.
         */
        static std::string extract_product_from_url(const std::string& url) {
            std::regex regexPattern("/listing/\\d+/([0-9a-z-]+)");
            std::smatch matches;

            if (std::regex_search(url, matches, regexPattern) && matches.size() > 1) {
                return matches[1].str();
            } else {
                return "No product found in the URL";
            }
        }


        /**
         * @brief Конвертирует строку в вектор unsigned char.
         * @param str Строка для конвертации.
         * @return Вектор unsigned char.
         */
        static std::vector<unsigned char> string_to_unsigned_char_vector(const std::string& str) {
            return std::vector<unsigned char>(str.begin(), str.end());
        }

        /**
         * @brief Выводит справочную информацию по использованию программы.
         */
        static void display_help() {
            std::cout << "Usage: etsy.exe [options]\n\n"
                    << "This program download images from cards with query frin etsy.com by proxy.\n\n"
                    << "To configure proxy list make file proxy_list.txt with all proxy servers in format protocol://username:password@ip:port \n\n"
                    << "Options:\n"
                    << "  --query, -q <query>     Specify the search query to be used.\n"
                    << "  --pages, -p <number>    Specify the number of pages to fetch.\n"
                    << "  --repars, -r <link>     Reparse already downloaded links from file parsed_links.txt in your directory\n"
                    << "  --help, -h              Display this help message and exit.\n\n"
                    << "Examples:\n"
                    << "  etsy.exe --query \"example query\" --pages 5\n"
                    << "  etsy.exe -q \"example\" -p 3\n"
                    << "  etsy.exe --repars \".\\parsed_links.txt\"\n\n"
                    << "Use --help to display this help information.\n";
        }


        /**
         * @brief Скачивает все изображения по ссылке, используя список валидных прокси.
         * @param url URL для скачивания изображений.
         * @param proxy_list Список валидных прокси.
         * @param repars Флаг повторного парсинга.
         */
        static void save_all_images_by_link(const std::string& url, const ProxyManager& proxy_list, bool repars = false){
            Downloader request;
            std::vector<unsigned char> readBuffer = request.download(url, proxy_list.validProxies);
            std::string html_code2 = convertVectorToString(readBuffer);
            std::vector<std::string> card = HtmlParser(html_code2).searchForAttribute("img", "src");
            std::vector<std::string> search_el = find_substring_in_vector(card, "etsystatic");
            search_el = find_substring_in_vector(search_el, "il");
            std::string url_img;

            std::string dirName = extract_product_from_url(url);
            std::cout << "Item name " << dirName << " Link: " << url << std::endl;
            
            if (CreateDirectory(dirName.c_str(), NULL)) {
                std::cout << "Directory created successfully." << std::endl;
                save_to_file(string_to_unsigned_char_vector(url), "parsed_links.txt", true);
            } else {
                if ((GetLastError() != 183) && (repars == false)){
                    std::cout << "hello bro" <<std::endl;
                    save_to_file(string_to_unsigned_char_vector(url), "parsed_links.txt", true);
                } else if (GetLastError() == 183){
                    std::cout << "Find directory " << dirName.c_str() << std::endl;
                } else{
                    std::cout << "Failed to create directory. Error: " << GetLastError() << std::endl;
                }
            }
            for(int i = 0; i < search_el.size(); i++){
                url_img = replace_image_size_with_regex(search_el[i], "1920xN");
                if(!url_img.empty()){
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    readBuffer = request.download(url_img,  proxy_list.validProxies);
                    save_to_file(readBuffer, dirName + "/img" + std::to_string(i) + ".jpg");
                    std::cout << "Saved to " << dirName + "/img" + std::to_string(i) + ".jpg" << std::endl;
                }
            }
        }

        /**
         * @brief Выполняет поиск по заданному запросу на заданное количество страниц.
         * @param query Поисковый запрос.
         * @param pages Количество страниц для поиска.
         * @return Успешность выполнения поиска.
         */
        static bool run_search(std::string query, int pages){
            std::vector<unsigned char> readBuffer;
            std::vector<std::string> parsed_links;
            ProxyManager proxy_list("../proxy_list.txt");
            Downloader request;
            if(query.empty()){
                std::cout << "Enter -q/--query for search" << std::endl;
                display_help();
                return false;
            }
            
            proxy_list.CreateValidProxies();

            for(int page = 1; page <= pages; page++){
                std::string url = "https://www.etsy.com/search?q=" + query + "&page=" + std::to_string(page);
                std::cout << proxy_list.validProxies[0] << std::endl;
                std::cout << "Start downloading images by proxy " << proxy_list.validProxies[0] << std::endl;
                readBuffer = request.download(url,  proxy_list.validProxies);
                std::string html_code = convertVectorToString(readBuffer);
                std::vector<std::string> search = HtmlParser(html_code).searchForAttribute("a", "href");
                std::vector<std::string> all_cards = find_substring_in_vector(search, "/listing/");
                for(const auto& link : all_cards){
                    save_all_images_by_link(link, proxy_list);
                }
            }
            return true;
        }
};
#endif // UTILS_H