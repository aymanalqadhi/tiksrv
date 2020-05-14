cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_shared)

# Files
set(SOURCE_FILES src/error.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
