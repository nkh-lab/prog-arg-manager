cmake_minimum_required(VERSION 3.4.1)

file(GLOB SRC_FILES "*.cpp")

add_executable(${PROJECT_NAME}-test ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}-test
    PRIVATE ${PROJECT_NAME}
    )
