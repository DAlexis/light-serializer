cmake_minimum_required(VERSION 2.8)

project(serialization-unit-tests)

find_package (GTest REQUIRED)
find_package (Threads)

include_directories (
    ${light-serializer_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIR}
)

set(EXE_SOURCES
    serialization-test.cpp
)

add_executable(${PROJECT_NAME} ${EXE_SOURCES})

target_link_libraries (${PROJECT_NAME}
    gtest
    gtest_main
    ${light-serializer_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

add_test(NAME SerializerTests
         COMMAND ${PROJECT_NAME})
run_test(${PROJECT_NAME})
