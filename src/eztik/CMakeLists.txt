cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik C)

# Files
set(SOURCE_FILES src/eztik.c
                 src/export.c
                 src/api_open.c
                 src/api_close.c
                 src/api_write.c
                 src/session.c
                 src/ros_api_open_command.c
                 src/ros_api_close_command.c
                 src/ros_version_command.c)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} ${LOG_PROJECT_NAME}
                                      ${SERVICES_PROJECT_NAME}
                                      ${COMMANDS_PROJECT_NAME})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
