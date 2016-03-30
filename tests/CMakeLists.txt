cmake_minimum_required(VERSION 2.8)

project(serialization-unit-tests)

find_package (GTest REQUIRED)

include_directories (
    ${serialization-utils_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIR}
)

set(EXE_SOURCES
    serialization-test.cpp
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

target_link_libraries (${PROJECT_NAME}
    ${serialization-utils_LIBRARY}
    gtest
    gtest_main
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

add_test(NAME SerializerTests
         COMMAND ${PROJECT_NAME})
run_test(${PROJECT_NAME})
