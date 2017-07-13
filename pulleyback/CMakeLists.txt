cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (pulleyback_erl)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Not yet. First, let's get the basics of an out-of-SteamWorks pulley-plugin
# sorted out; that's easier to do without the extra checks for Erlang.
#
# find_package(Erlang REQUIRED)

add_subdirectory (src)
