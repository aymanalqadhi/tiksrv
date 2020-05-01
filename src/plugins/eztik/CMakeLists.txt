cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik)

# Files
set(SOURCE_FILES src/eztik_plugin.cpp)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} tiksrv_interop)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION plugins)
