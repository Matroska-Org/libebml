cmake_minimum_required(VERSION 3.5)

project(linkebml VERSION 0.0.1)

find_package(EBML REQUIRED)

add_executable(testlink test00.cpp)
target_link_libraries(testlink PUBLIC EBML::ebml)


