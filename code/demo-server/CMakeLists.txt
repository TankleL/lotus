cmake_minimum_required(VERSION 3.10)
message(STATUS "making demo-server ...")

add_executable(demoserver
    main.cxx
)

target_include_directories(demoserver PRIVATE ${LIBLOTUS_INCLUDE})
target_link_libraries(demoserver
    PRIVATE lotus_a
)

message(STATUS "make demo-server - done")

