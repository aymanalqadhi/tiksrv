cmake_minimum_required(VERSION 3.10)

# Project Name
project(${SHARED_PROJECT_NAME} C)

# Project files
set(SOURCE_FILES src/string.c)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
