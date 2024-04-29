#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <vector>
#include <string>
#include <gumbo.h>

/**
 * @brief Класс для парсинга HTML документов.
 */
class HtmlParser {
public:
    /**
     * @brief Конструктор класса HtmlParser.
     * 
     * @param html Строка, содержащая HTML документ для парсинга.
     */
    HtmlParser(const std::string& html);

    /**
     * @brief Деструктор класса HtmlParser.
     */
    ~HtmlParser();

    /**
     * @brief Ищет атрибуты заданного тега в HTML документе.
     * 
     * @param tag_name Имя тега, для которого нужно найти атрибуты.
     * @param attribute_name Имя атрибута, который необходимо найти.
     * @return std::vector<std::string> Список значений найденных атрибутов.
     */
    std::vector<std::string> searchForAttribute(const std::string& tag_name, const std::string& attribute_name);

private:
    GumboOutput* output; ///< Структура GumboOutput, содержащая результаты парсинга HTML.

    /**
     * @brief Рекурсивно ищет атрибуты в узлах HTML документа.
     * 
     * @param node Указатель на узел GumboNode.
     * @param tag_name Имя тега, для которого нужно найти атрибуты.
     * @param attribute_name Имя атрибута, который необходимо найти.
     * @param results Вектор для сохранения найденных значений атрибутов.
     */
    void searchForAttribute(const GumboNode* node, const std::string& tag_name, const std::string& attribute_name, std::vector<std::string>& results);
};

#endif // HTML_PARSER_H
