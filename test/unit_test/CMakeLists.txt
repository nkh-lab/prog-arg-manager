cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

find_package(GTest REQUIRED)
find_package(Threads)

set(UNIT_TEST_NAME ${PROJECT_NAME}-utest)

file(GLOB SRC_FILES "*.cpp")

add_executable(${UNIT_TEST_NAME} ${SRC_FILES})

target_link_libraries(${UNIT_TEST_NAME}
    PRIVATE ${PROJECT_NAME}
    PRIVATE ${GTEST_LIBRARIES}
    PRIVATE GTest::Main
    PRIVATE ${CMAKE_THREAD_LIBS_INIT}
    )

