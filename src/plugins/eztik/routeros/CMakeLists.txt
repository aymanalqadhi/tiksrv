cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_routeros)

# Files
set(SOURCE_FILES src/api.cpp
                 src/api_read_state_machine.cpp
                 src/sentence.cpp
                 src/util.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Depencencies
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} eztik_shared
                                      tiksrv_log
                                      cryptopp
                                      ${Boost_LIBRARIES})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
