cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik)

# Files
set(SOURCE_FILES src/eztik_plugin.cpp
                 src/routeros_api.cpp
                 src/routeros_api_read_state_machine.cpp
                 src/routeros_util.cpp
                 src/routeros_sentence.cpp
                 src/session.cpp
                 src/sessions_service.cpp
                 src/session_required_command.cpp
                 src/api_open_command.cpp
                 src/api_send_command.cpp)

# Targets
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} tiksrv_interop cryptopp)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION plugins)
