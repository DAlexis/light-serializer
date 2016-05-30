cmake_minimum_required(VERSION 2.8)

project(light-serializer)

set(LIB_SOURCE serialization.cpp)

set(${PROJECT_NAME}_USED_INCDIRS
    ${utils_INCLUDE_DIRS}
)

include_directories(
    ${${PROJECT_NAME}_USED_INCDIRS}
)

add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_rvalue_references)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_lambdas)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_constexpr)

export_include_dirs(
    ${PROJECT_SOURCE_DIR}
    ${${PROJECT_NAME}_USED_INCDIRS}
)
