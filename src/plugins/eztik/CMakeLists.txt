cmake_minimum_required(VERSION 3.10)

# Project Options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DBOOST_ASIO_DISABLE_CONCEPTS)

# Sub directories inclusion
add_subdirectory(shared)
add_subdirectory(routeros)
add_subdirectory(data)
add_subdirectory(services)
add_subdirectory(commands)
add_subdirectory(eztik)
