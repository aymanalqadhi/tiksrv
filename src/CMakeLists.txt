cmake_minimum_required(VERSION 3.10)

# Project name
project(tiksrv)

# Options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

# Sub directories inclusion
add_subdirectory(app)
add_subdirectory(log)
add_subdirectory(main)
add_subdirectory(lib)

# Final executable
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:tiksrv_main>)
target_link_libraries(${PROJECT_NAME} tiksrv_app)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
