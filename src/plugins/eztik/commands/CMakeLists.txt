cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_commands)

# Files
set(SOURCE_FILES src/session_required_command.cpp
                 src/api_open_command.cpp
                 src/api_send_command.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} eztik_routeros
                                      eztik_services
                                      tiksrv_net
                                      tiksrv_interop)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
