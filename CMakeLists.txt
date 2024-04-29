cmake_minimum_required(VERSION 3.10)

project(robot_etsy)


include_directories(${VCPKG_INCLUDE_DIR})
include_directories(include)

add_executable(robot_etsy src/main.cpp src/ProxyManager.cpp src/Downloader.cpp src/HTMLParser.cpp include/Utils.h)
find_package(CURL REQUIRED)
target_link_libraries(robot_etsy PRIVATE CURL::libcurl)
find_package(unofficial-gumbo CONFIG REQUIRED)
target_link_libraries(robot_etsy PRIVATE unofficial::gumbo::gumbo)
install(TARGETS robot_etsy DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

