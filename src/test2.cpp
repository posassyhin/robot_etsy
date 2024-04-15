#include <gumbo.h>
#include <thread>
#include <chrono>
#include <regex>
#include <windows.h>
#include "ProxyManager.h"
#include "Downloader.h"
#include "HTMLParser.h"


std::string convertVectorToString(const std::vector<unsigned char>& data) {
    std::string result(data.begin(), data.end());
    return result;
}

std::vector<std::string> get_parsed_links(const std::string& filename){
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


std::vector<std::string> find_substring_in_vector(const std::vector<std::string>& input, const std::string& substring) {
    std::vector<std::string> results;
    for (const auto& item : input) {
        if (item.find(substring) != std::string::npos) {
            results.push_back(item);
        }
    }
    return results;
}


std::string replace_image_size_with_regex(const std::string& url, const std::string& newSize) {
    std::regex sizeRegex(R"(\d+xN)");

    std::string modifiedUrl = std::regex_replace(url, sizeRegex, newSize);

    if (modifiedUrl == url) {
        return "";
    }

    return modifiedUrl;
}


bool save_to_file(const std::vector<unsigned char>& imageData, const std::string& filepath, bool append = false) {
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


std::string extract_product_from_url(const std::string& url) {
    std::regex regexPattern("/listing/\\d+/([0-9a-z-]+)");
    std::smatch matches;

    if (std::regex_search(url, matches, regexPattern) && matches.size() > 1) {
        return matches[1].str();
    } else {
        return "No product found in the URL";
    }
}

std::vector<unsigned char> string_to_unsigned_char_vector(const std::string& str) {
    return std::vector<unsigned char>(str.begin(), str.end());
}

void display_help() {
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

void save_all_images_by_link(const std::string& url, const ProxyManager& proxy_list, bool repars = false){
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

bool run_search(std::string query, int pages){
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