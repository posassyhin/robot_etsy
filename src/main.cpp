#include "Utils.h"  // Подключение созданного заголовочного файла
#include <iostream>

int main(int argc, char* argv[]) {
    std::string query;
    int pages = 1;
    std::vector<std::string> parsed_links;
    ProxyManager proxy_list("../proxy_list.txt");

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--help" || arg == "-h")) {
            display_help();
            return 0;
        } else if ((arg == "--query" || arg == "-q") && i + 1 < argc) {
            query = argv[++i];
            if ((arg == "--pages" || arg == "-p") && i + 1 < argc) {
                pages = std::stoi(argv[++i]);
            }
            run_search(query, pages);
        } else if ((arg == "--repars" || arg == "-r") && i + 1 < argc) {
            proxy_list.CreateValidProxies();
            parsed_links = get_parsed_links(argv[++i]);
            for(const auto& link : parsed_links) {
                save_all_images_by_link(link, proxy_list, true);
            }
            return 0;
        } else {
            std::cerr << "Unknown or incomplete argument: " << arg << std::endl;
            display_help();
            return 1;
        }
    }
}