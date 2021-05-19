## premake.cmake
# lotus engineering system

set(LOTUSES_VCPKG_TRIPLET_DEFAULT "x64-windows-static")
if(UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    set(LOTUSES_VCPKG_TRIPLET_DEFAULT "x64-linux")
endif()

# require LOTUSES_VCPKG_ROOT
if(NOT DEFINED LOTUSES_VCPKG_ROOT OR
    LOTUSES_VCPKG_ROOT STREQUAL "")
    message(FATAL_ERROR "missing LOTUSES_VCPKG_ROOT; e.g. /home/dev/vcpkg or C:\\dev\\vcpkg")
else()
    message(STATUS "vcpkg root: ${LOTUSES_VCPKG_ROOT}")
endif()

# require LOTUSES_VCPKG_TRIPLET
if(NOT DEFINED LOTUSES_VCPKG_TRIPLET OR
    LOTUSES_VCPKG_TRIPLET STREQUAL "")
    message(WARNING "missing LOTUSES_VCPKG_TRIPLET, using ${LOTUSES_VCPKG_TRIPLET_DEFAULT} by default")
    set(LOTUSES_VCPKG_TRIPLET ${LOTUSES_VCPKG_TRIPLET_DEFAULT})
else()
    message(STATUS "vckpg triplet: ${LOTUSES_VCPKG_TRIPLET}")
endif()

# check if vcpkg.cmake exists
set(LOTUSES_VCPKG_CMAKE "${LOTUSES_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
if(EXISTS "${LOTUSES_VCPKG_CMAKE}")
    message(STATUS "found vcpkg.cmake: ${LOTUSES_VCPKG_CMAKE}")
else()
    message(FATAL_ERROR "vcpkg.cmake not found: ${LOTUSES_VCPKG_CMAKE}")
endif()

# run vcpkg.cmake
message(STATUS "running vcpkg.cmake ...")
set(VCPKG_TARGET_TRIPLET ${LOTUSES_VCPKG_TRIPLET})
include(${LOTUSES_VCPKG_CMAKE})
message(STATUS "run vcpkg.cmake - done")

# find required packages
message(STATUS "running findpkgs.cmake")
include(${CMAKE_CURRENT_LIST_DIR}/findpkgs.cmake)
message(STATUS "run findpkgs.cmake - done")


