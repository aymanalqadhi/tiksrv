cmake_minimum_required(VERSION 3.10)

# Project Name
project(tiksrv_interop)

# Files
set(SOURCE_FILES src/plugin_loader.cpp)

# Targets
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

# Dependencies
find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} tiksrv_services
                                      tiksrv_net
                                      ${Boost_LIBRARIES}
                                      dl)

# Install destination
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
