cmake_minimum_required(VERSION 3.10)

message(STATUS "lotus engineering system: find required packages ...")

find_package(Threads)
find_package(uvw CONFIG REQUIRED)
find_package(msgpack CONFIG REQUIRED)

message(STATUS "lotus engineering system: find required packages - done")
