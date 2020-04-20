cmake_minimum_required(VERSION 3.10)

# Project name
project(tiksrv C)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Options
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

# External libraries config
include(cmake/FindGLIB.cmake)
include_directories(${GLIB_INCLUDE_DIRS})

# Paths
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

# Sub Projects
set(MAIN_PROJECT_NAME ${PROJECT_NAME}_main)
set(CONFIG_PROJECT_NAME ${PROJECT_NAME}_config)
set(LOG_PROJECT_NAME ${PROJECT_NAME}_log)
set(NET_PROJECT_NAME ${PROJECT_NAME}_net)
set(APP_PROJECT_NAME ${PROJECT_NAME}_app)
set(SERVICES_PROJECT_NAME ${PROJECT_NAME}_services)
set(COMMANDS_PROJECT_NAME ${PROJECT_NAME}_commands)
set(INTEROP_PROJECT_NAME ${PROJECT_NAME}_interop)
set(TESTS_PROJECT_NAME ${PROJECT_NAME}_tests)
set(SHARED_PROJECT_NAME ${PROJECT_NAME}_shared)

add_subdirectory(main)
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(net)
add_subdirectory(app)
add_subdirectory(services)
add_subdirectory(commands)
add_subdirectory(interop)
add_subdirectory(shared)
add_subdirectory(tests)
add_subdirectory(lib)

# Plugins
add_subdirectory(eztik)

# Final executable
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:${MAIN_PROJECT_NAME}>)

# Dependencies
set(EXTERNAL_DEPS uv ezd ${GLIB_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${CONFIG_PROJECT_NAME}
                                      ${LOG_PROJECT_NAME}
                                      ${NET_PROJECT_NAME}
                                      ${APP_PROJECT_NAME}
                                      ${SERVICES_PROJECT_NAME}
                                      ${COMMANDS_PROJECT_NAME}
                                      ${INTEROP_PROJECT_NAME}
                                      ${SHARED_PROJECT_NAME}
                                      ${EXTERNAL_DEPS})

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
