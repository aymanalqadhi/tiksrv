cmake_minimum_required(VERSION 3.10)

# Project name
project(eztik)

# Project files
set(SOURCE_FILES src/plugin.cpp)
include_directories(include)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Dependencies
target_link_libraries(${PROJECT_NAME} eztik_shared
                                      eztik_services
                                      eztik_commands
                                      tiksrv_interop)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION plugins)
