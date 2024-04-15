#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <vector>
#include <string>
#include <gumbo.h>

class HtmlParser {
public:
    HtmlParser(const std::string& html);
    ~HtmlParser();
    std::vector<std::string> searchForAttribute(const std::string& tag_name, const std::string& attribute_name);

private:
    GumboOutput* output;

    void searchForAttribute(const GumboNode* node, const std::string& tag_name, const std::string& attribute_name, std::vector<std::string>& results);
};

#endif // HTML_PARSER_H
