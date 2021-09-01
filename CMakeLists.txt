cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

project(prog-arg-manager)

option(${PROJECT_NAME}_BUILD_TESTS "Build tests" off)
option(${PROJECT_NAME}_BUILD_UNITTESTS "Build unit tests" off)

add_subdirectory(src)

if (${PROJECT_NAME}_BUILD_TESTS )
    add_subdirectory(test/component_test)
endif()

if (${PROJECT_NAME}_BUILD_UNITTESTS)
    add_subdirectory(test/unit_test)
endif()

