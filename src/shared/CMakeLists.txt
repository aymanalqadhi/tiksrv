cmake_minimum_required(VERSION 3.10)

# Project Name
project(${SHARED_PROJECT_NAME} C)

# Targets
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
