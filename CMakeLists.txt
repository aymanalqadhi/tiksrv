cmake_minimum_required(VERSION 3.10)

project(tiksrv C)

# Projct Files
set(SOURCE_FILES src/main.c src/config.c src/error.c src/log.c src/net_address.c
                 src/net_tcp_listener.c src/net_tcp_listener_callbacks.c
                 src/net_tcp_client.c src/net_tcp_client_callbacks.c
                 src/net_memory.c src/app.c)
include_directories(include)

# Project Targets
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} uv)
