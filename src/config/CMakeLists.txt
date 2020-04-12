cmake_minimum_required(VERSION 3.10)

# Project Name
project(${CONFIG_PROJECT_NAME} C)

# Files
set(SOURCE_FILES src/config.c)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
