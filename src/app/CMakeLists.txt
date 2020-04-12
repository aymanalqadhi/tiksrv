cmake_minimum_required(VERSION 3.10)

# Project Name
project(${APP_PROJECT_NAME} C)

# Files
set(SOURCE_FILES src/app.c src/callbacks.c)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} ${CONFIG_PROJECT_NAME}
                                      ${LOG_PROJECT_NAME}
                                      ${NET_PROJECT_NAME}
                                      ${SHARED_PROJECT_NAME})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
