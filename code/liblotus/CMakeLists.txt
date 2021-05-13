cmake_minimum_required(VERSION 3.10)
message(STATUS "making liblotus ...")

set(LIBLOTUS_INCLUDE_FILES
    inc/session.hxx
)

set(LIBLOTUS_SOURCE_FILES
    src/session.cxx
)

add_library(lotus_a STATIC
    ${LIBLOTUS_SOURCE_FILES}
    ${LIBLOTUS_INCLUDE_FILES} # for the convienece of searching in IDE
)

set_target_properties(lotus_a PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(lotus_a PRIVATE
    "inc"
)

message(STATUS "make liblotus - done")
