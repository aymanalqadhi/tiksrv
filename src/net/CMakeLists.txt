cmake_minimum_required(VERSION 3.10)

# Project Name
project(tiksrv_net)

# Files
set(SOURCE_FILES src/tcp_server.cpp
                 src/tcp_client.cpp
                 src/message.cpp
                 src/read_state_machine.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} tiksrv_log ${Boost_LIBRARIES})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
