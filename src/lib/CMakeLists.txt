cmake_minimum_required(VERSION 3.10)

# uthash
add_library(uthash INTERFACE)
target_include_directories(uthash INTERFACE uthash/include/)

# libuv
add_subdirectory(libuv)

# Unity
add_subdirectory(Unity)

# ezd
add_subdirectory(ezd)
