cmake_minimum_required(VERSION 3.10)
message(STATUS "making liblotus ...")

set(LIBLOTUS_INCLUDE_FILES
    inc/conn.hxx
    inc/connection/httpconn-cli.hxx
    inc/connection/httpconn-svr.hxx
    inc/connection/tcpconn-cli.hxx
    inc/connection/tcpconn-svr.hxx
    inc/msg.hxx
    inc/session.hxx
    inc/staloop.hxx
    inc/uuid.hxx
)

set(LIBLOTUS_SOURCE_FILES
    src/connection/tcpconn-cli.cxx
    src/msg.cxx
    src/session.cxx
    src/staloop.cxx
    src/uuid.cxx
)

add_library(lotus_a STATIC
    ${LIBLOTUS_SOURCE_FILES}
    ${LIBLOTUS_INCLUDE_FILES} # for the convienece of searching in IDE
)

target_include_directories(lotus_a PRIVATE
    "inc"
    "inc/connection"
)

target_link_libraries(lotus_a
    PRIVATE uvw::uvw
)

message(STATUS "make liblotus - done")
