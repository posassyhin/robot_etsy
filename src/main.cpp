#include "Utils.h"  // Подключение созданного заголовочного файла

int main() {
    // Пример использования метода convertVectorToString
    std::vector<unsigned char> data = {'H', 'e', 'l', 'l', 'o'};
    std::string resultString = Utils::convertVectorToString(data);
    std::cout << "Преобразованная строка: " << resultString << std::endl;

    // Пример использования метода get_parsed_links
    std::string filename = "links.txt";
    std::vector<std::string> links = Utils::get_parsed_links(filename);
    std::cout << "Считанные ссылки:" << std::endl;
    for (const auto& link : links) {
        std::cout << link << std::endl;
    }

    // Пример использования метода save_to_file
    std::string filepath = "output.bin";
    if (Utils::save_to_file(data, filepath, false)) {
        std::cout << "Данные успешно сохранены в файл " << filepath << std::endl;
    } else {
        std::cout << "Ошибка при сохранении данных." << std::endl;
    }

    // Пример использования метода display_help
    Utils::display_help();

    // Пример использования метода run_search
    std::string query = "example query";
    int pages = 3;
    if (Utils::run_search(query, pages)) {
        std::cout << "Поиск выполнен успешно." << std::endl;
    } else {
        std::cout << "Ошибка при выполнении поиска." << std::endl;
    }

    return 0;
}