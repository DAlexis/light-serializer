cmake_minimum_required(VERSION 3.1)

project(serialization-unit-tests)

set(EXE_SOURCES
    serialization-test.cpp
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

target_link_libraries (${PROJECT_NAME}
    gtest
    gtest_main
    light-serializer
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)

add_test(NAME SerializerTests COMMAND ${PROJECT_NAME})
