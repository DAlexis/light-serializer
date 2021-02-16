cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(light-serializer-base)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g -DNDEBUG")

# To enable ctest usage
enable_testing()
add_subdirectory(serializer)
include(detect-gtest.cmake)
if(GTEST_FOUND)
	add_subdirectory(tests)
endif()

