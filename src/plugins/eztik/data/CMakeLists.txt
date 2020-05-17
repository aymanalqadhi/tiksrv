cmake_minimum_required(VERSION 3.10)

# Project Name
project(eztik_data)

# Files
set(SOURCE_FILES src/dummy.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Depencencies
target_link_libraries(${PROJECT_NAME} eztik_routeros)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
