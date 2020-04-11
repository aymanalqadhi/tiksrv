cmake_minimum_required(VERSION 3.10)

project(tiksrc C)

# Projct Files
set(SOURCE_FILES src/main.c)

# Project Targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
