cmake_minimum_required(VERSION 3.4.1)

set(COMPONENT_TEST_NAME ${PROJECT_NAME}-ctest)

file(GLOB SRC_FILES "*.cpp")

add_executable(${COMPONENT_TEST_NAME} ${SRC_FILES})

target_link_libraries(${COMPONENT_TEST_NAME}
    PRIVATE ${PROJECT_NAME}
    )
