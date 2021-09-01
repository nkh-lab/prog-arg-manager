cmake_minimum_required(VERSION 3.4.1)

find_package(Boost 1.65 REQUIRED COMPONENTS program_options)

file(GLOB SRC_FILES "*.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ../include)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Boost::program_options
    )
