cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_commands)

# Protobuf files
set(GENERATED_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated)

set(PROTO_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(PROTO_OUT_DIR ${GENERATED_FILES_DIR}/proto)
set(PROTO_INTERFACE_DIR ${GENERATED_FILES_DIR})

execute_process(COMMAND make -C ${PROTO_IN_DIR}
                        OUTPUT_DIR=${PROTO_OUT_DIR}
                RESULT_VARIABLE ret)

if(NOT ret EQUAL 0)
    message(FATAL_ERROR "Could not compile protbuf files")
endif()

file(GLOB PROTO_FILES ${PROTO_OUT_DIR}/*.cc
                      ${PROTO_OUT_DIR}/*/*.cc)

# Files
set(SOURCE_FILES src/session_required_command.cpp
                 src/api_open_command.cpp
                 src/api_send_command.cpp
                 ${PROTO_FILES})

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include
                                                  ${PROTO_INTERFACE_DIR})

# Dependencies
target_link_libraries(${PROJECT_NAME} eztik_routeros
                                      eztik_data
                                      eztik_services
                                      tiksrv_net
                                      tiksrv_interop
                                      protobuf::libprotobuf)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
