cmake_minimum_required(VERSION 3.15)

project(cpp-backstack VERSION 1.0.0)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

file(GLOB_RECURSE SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

option(BACKSTACK_BUILD_TESTS "Build tests" ON)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

if(CMAKE_HOST_UNIX)
    target_link_libraries(${PROJECT_NAME} INTERFACE unwind)
endif()

if(BACKSTACK_BUILD_TESTS)
    add_subdirectory(test)
endif()
