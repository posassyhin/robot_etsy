#include "HtmlParser.h"

HtmlParser::HtmlParser(const std::string& html) {
    output = gumbo_parse(html.c_str());
}

HtmlParser::~HtmlParser() {
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

std::vector<std::string> HtmlParser::searchForAttribute(const std::string& tag_name, const std::string& attribute_name) {
    std::vector<std::string> results;
    searchForAttribute(output->root, tag_name, attribute_name, results);
    return results;
}

void HtmlParser::searchForAttribute(const GumboNode* node, const std::string& tag_name, const std::string& attribute_name, std::vector<std::string>& results) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboTag target_tag = gumbo_tag_enum(tag_name.c_str());
    if (node->v.element.tag == target_tag) {
        GumboAttribute* attribute = gumbo_get_attribute(&node->v.element.attributes, attribute_name.c_str());
        if (attribute) {
            results.push_back(attribute->value);
        }
    }

    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        searchForAttribute(static_cast<const GumboNode*>(children->data[i]), tag_name, attribute_name, results);
    }
}
