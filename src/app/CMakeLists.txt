cmake_minimum_required(VERSION 3.10)

# Project Name
project(tiksrv_app)

# Files
set(SOURCE_FILES src/app.cpp
                 src/server.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} tiksrv_config
                                      tiksrv_log
                                      tiksrv_net
                                      tiksrv_services
                                      tiksrv_interop)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
