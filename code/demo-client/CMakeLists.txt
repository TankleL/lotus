cmake_minimum_required(VERSION 3.10)
message(STATUS "making demo-client ...")

add_executable(democlient
    main.cxx
)

target_include_directories(democlient PRIVATE ${LIBLOTUS_INCLUDE})
target_link_libraries(democlient
    PRIVATE lotus_a
)

message(STATUS "make demo-client - done")

