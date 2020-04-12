cmake_minimum_required(VERSION 3.10)

# Project name
project(tiksrv C)

# Options
set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Paths
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

# Sub Projects
set(MAIN_PROJECT_NAME ${PROJECT_NAME}_main)
set(CONFIG_PROJECT_NAME ${PROJECT_NAME}_config)
set(LOG_PROJECT_NAME ${PROJECT_NAME}_log)
set(NET_PROJECT_NAME ${PROJECT_NAME}_net)
set(APP_PROJECT_NAME ${PROJECT_NAME}_app)
set(SHARED_PROJECT_NAME ${PROJECT_NAME}_shared)

add_subdirectory(main)
add_subdirectory(config)
add_subdirectory(log)
add_subdirectory(net)
add_subdirectory(app)
add_subdirectory(shared)

# Final executable
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:${MAIN_PROJECT_NAME}>)

# Dependencies
set(EXTERNAL_DEPS uv)
target_link_libraries(${PROJECT_NAME} ${CONFIG_PROJECT_NAME}
                                      ${LOG_PROJECT_NAME}
                                      ${NET_PROJECT_NAME}
                                      ${APP_PROJECT_NAME}
                                      ${SHARED_PROJECT_NAME}
                                      ${EXTERNAL_DEPS})

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
