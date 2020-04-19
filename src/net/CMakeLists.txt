cmake_minimum_required(VERSION 3.10)

# Project Name
project(${NET_PROJECT_NAME} C)

# Files
set(SOURCE_FILES src/memory.c src/address.c src/message.c src/tcp_client.c
                 src/tcp_listener.c src/read_state_machine.c
                 src/tcp_client_callbacks.c src/tcp_listener_callbacks.c)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} ${CONFIG_PROJECT_NAME}
                                      ${LOG_PROJECT_NAME}
                                      ${SHARED_PROJECT_NAME} ezd)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
