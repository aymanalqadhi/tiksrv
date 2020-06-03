cmake_minimum_required(VERSION 3.10)

# Project name
project(tiksrv)

# Project Options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS -rdynamic)
add_definitions(-DBOOST_ASIO_DISABLE_CONCEPTS)

# Options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# Paths
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)

# Sub directories inclusion
add_subdirectory(main)
add_subdirectory(app)
add_subdirectory(interop)
add_subdirectory(services)
add_subdirectory(net)
add_subdirectory(log)
add_subdirectory(config)

add_subdirectory(plugins)
add_subdirectory(lib)
add_subdirectory(html)


# Final executable
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:tiksrv_main>)
target_link_libraries(${PROJECT_NAME} tiksrv_app)

# Install
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
