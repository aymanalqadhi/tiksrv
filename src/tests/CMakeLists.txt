cmake_minimum_required(VERSION 3.10)

# Project name
project(${TESTS_PROJECT_NAME} C)

set(TESTS_ITERATIONS 1000000)
add_definitions(-DTEST_ITERATIONS=${TESTS_ITERATIONS})
set(COMPILE_FLAGS -pg)

# Project functions
function(create_test test_name)
    add_executable(${test_name} ${test_name}.c)
    target_link_libraries(${test_name} unity)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Project targets
create_test(message_tests)
target_link_libraries(message_tests ${NET_PROJECT_NAME})
