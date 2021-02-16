cmake_minimum_required(VERSION 3.1)

project(light-serializer)

set(LIB_SOURCE
    serialization.cpp
    serialization.hpp
)

include_directories(
    ${${PROJECT_NAME}_USED_INCDIRS}
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11)
