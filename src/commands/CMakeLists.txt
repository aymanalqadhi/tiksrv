cmake_minimum_required(VERSION 3.10)

# Project Name
project(${COMMANDS_PROJECT_NAME} C)

# Files
set(SOURCE_FILES src/request.c src/response.c src/command.c src/echo_command.c)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} ${LOG_PROJECT_NAME}
                                      ${NET_PROJECT_NAME}
                                      ${SERVICES_PROJECT_NAME}
                                      ${SHARED_PROJECT_NAME}
                                      uv m)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
