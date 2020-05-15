# spdlog
add_subdirectory(spdlog)

# fmt
add_subdirectory(fmt)

# protobuf
add_definitions(-Dprotobuf_BUILD_TESTS=OFF)
add_subdirectory(protobuf/cmake)
