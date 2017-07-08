cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (ceci)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(Erlang REQUIRED)

add_subdirectory (src)
