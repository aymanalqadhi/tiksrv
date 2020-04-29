cmake_minimum_required(VERSION 3.10)

# Project Name
project(tiksrv_app)

# Files
set(SOURCE_FILES src/config.cpp src/app.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
