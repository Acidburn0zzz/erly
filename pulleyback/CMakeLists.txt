cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (pulleyback_erl)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Not yet. First, let's get the basics of an out-of-SteamWorks pulley-plugin
# sorted out; that's easier to do without the extra checks for Erlang.
#
# find_package(Erlang REQUIRED)
find_package (Steamworks 0.8 REQUIRED)

include_directories (${Steamworks_INCLUDE_DIRS})

add_subdirectory (src)
