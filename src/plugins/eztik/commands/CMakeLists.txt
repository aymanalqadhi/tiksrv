cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_commands)

# Protobuf files
execute_process(COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/proto
                RESULT_VARIABLE ret)

if(NOT ret EQUAL 0)
    message(FATAL_ERROR "Could not compile protbuf files")
endif()

file(GLOB PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/proto/*.cc)
include_directories(src/impl/proto/)

# Files
set(SOURCE_FILES src/session_required_command.cpp
                 src/api_open_command.cpp
                 src/api_send_command.cpp
                 src/api_load_hotspot_users_command.cpp
                 ${PROTO_FILES})

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
target_link_libraries(${PROJECT_NAME} eztik_routeros
                                      eztik_data
                                      eztik_services
                                      tiksrv_net
                                      tiksrv_interop
                                      protobuf::libprotobuf)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
