cmake_minimum_required(VERSION 3.10)

# Project Name
project(${MAIN_PROJECT_NAME} C)

# Files
set(SOURCE_FILES src/main.c)

# Targets
add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CONFIG_PROJECT_NAME}
                                      ${LOG_PROJECT_NAME}
                                      ${APP_PROJECT_NAME})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
