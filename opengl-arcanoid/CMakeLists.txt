cmake_minimum_required(VERSION 3.10)
project(arcanoid)

include(${CMAKE_SOURCE_DIR}/include/arch.cmake)
target_architecture(ARCHITECTURE)

if(${ARCHITECTURE} MATCHES "x86_64")
    set(TARGET_BITS 64)
else()
    set(TARGET_BITS 32)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 17)
endif ()

include_directories(${CMAKE_BINARY_DIR}/include)

FILE(GLOB SRCFILES ${CMAKE_BINARY_DIR}/src/*.cpp)

add_executable(arcanoid ${SRCFILES})
target_link_libraries(arcanoid ${CMAKE_BINARY_DIR}/lib${TARGET_BITS}/freeglut.lib)

MESSAGE(${TARGET_BITS})
