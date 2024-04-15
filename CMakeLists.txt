cmake_minimum_required(VERSION 3.10)

project(robot_etsy)



find_package(CURL REQUIRED)

include_directories(${VCPKG_INCLUDE_DIR})
include_directories(include)

add_executable(robot_etsy src/test2.cpp src/ProxyManager.cpp src/Downloader.cpp src/HTMLParser.cpp)

target_link_libraries(robot_etsy PRIVATE CURL::libcurl)
find_package(unofficial-gumbo CONFIG REQUIRED)
target_link_libraries(robot_etsy PRIVATE unofficial::gumbo::gumbo)

