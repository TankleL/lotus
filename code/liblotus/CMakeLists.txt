cmake_minimum_required(VERSION 3.10)
message(STATUS "making liblotus ...")

set(LIBLOTUS_INCLUDE_FILES
    inc/attachable.hxx
    inc/conn.hxx
    inc/connection/httpconn-cli.hxx
    inc/connection/httpconn-svr.hxx
    inc/connection/tcpconn-cli.hxx
    inc/connection/tcpconn-svr.hxx
    inc/msg.hxx
    inc/session.hxx
    inc/session-lstnr.hxx
    inc/staloop.hxx
    inc/uuid.hxx
)

set(LIBLOTUS_INCLUDE_INTERNAL_FILES
    inc-intl/connstr.hxx
    inc-intl/staloop-intl.hxx
)

set(LIBLOTUS_SOURCE_FILES
    src/attachable.cxx
    src/connection/tcpconn-cli.cxx
    src/connection/tcpconn-svr.cxx
    src/connstr.cxx
    src/msg.cxx
    src/session.cxx
    src/session-lstnr.cxx
    src/staloop.cxx
    src/staloop-intl.cxx
    src/uuid.cxx
)

set(LIBLOTUS_PROTOCOL_FILES
    protocols/protobase.cxx
    protocols/protobase.hxx
    protocols/proto-session-lstnr.cxx
    protocols/proto-session-lstnr.hxx
)

add_library(lotus_a STATIC
    ${LIBLOTUS_PROTOCOL_FILES}
    ${LIBLOTUS_SOURCE_FILES}
    ${LIBLOTUS_INCLUDE_FILES} # for the convienece of searching in IDE
    ${LIBLOTUS_INCLUDE_INTERNAL_FILES} # for the convienece of searching in IDE
)

target_include_directories(lotus_a PRIVATE
    "inc"
    "inc/connection"
    "inc-intl"
)

target_link_libraries(lotus_a
    PRIVATE uvw::uvw
    PRIVATE dl
    PRIVATE msgpackc msgpackc-cxx
)

message(STATUS "make liblotus - done")
