# Version CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(widget)

# Option
set(CMAKE_CXX_STANDARD 17)

# Compiler-specific options
if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -Werror
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Source files
file(GLOB_RECURSE SOURCES
    source/*.cpp
)

find_package(OpenCV REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Boost components
set(BOOST_COMPONENTS
    program_options
    system
    filesystem
)
find_package(Boost COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

find_package(Threads)

# Compiling target
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    stdc++fs
    ${OpenCV_LIBS}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
