cmake_minimum_required(VERSION 3.10)
message(STATUS "making project lotus ...")

# project freeisa
project(lotus)

# run premake.cmake
message(STATUS "running premake.cmake ...")
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/premake.cmake)
message(STATUS "run premake.cmake - done")

# normalize useful directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# use C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set important include paths
set(LIBLOTUS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/liblotus/inc")

# sub directories
add_subdirectory(liblotus)
add_subdirectory(demo-client)
add_subdirectory(demo-server)

message(STATUS "make project lotus - done")
