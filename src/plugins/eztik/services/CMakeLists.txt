cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_services)

# Files
set(SOURCE_FILES src/session.cpp
                 src/sessions_service.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} eztik_routeros
                                      tiksrv_config
                                      tiksrv_net
                                      tiksrv_services)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
