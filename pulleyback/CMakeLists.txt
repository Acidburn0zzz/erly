cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (pulleyback_erl)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package (Erlang REQUIRED)
find_package (Steamworks 0.8 REQUIRED)

include_directories (${Steamworks_INCLUDE_DIRS} ${Erlang_INCLUDE_DIRS})

add_subdirectory (src)
