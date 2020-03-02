# - Try to find Leveldb includes dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(Leveldb)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
# Variables defined by this module:
#
#  Leveldb_FOUND            System has Leveldb, include and lib dirs found
#  Leveldb_INCLUDE_DIR      The Leveldb includes directories.
#  Leveldb_LIBRARY          The Leveldb library.

find_path(Leveldb_INCLUDE_DIR NAMES leveldb)
find_library(Leveldb_LIBRARY NAMES libleveldb.a)

if(Leveldb_INCLUDE_DIR AND Leveldb_LIBRARY)
    set(Leveldb_FOUND TRUE)
    mark_as_advanced(
        Leveldb_INCLUDE_DIR
        Leveldb_LIBRARY
    )
endif()

if(Leveldb_FOUND)
    message(STATUS "Leveldb found!")
endif()

if(NOT Leveldb_FOUND)
    message(FATAL_ERROR "Leveldb doesn't exist")
endif()

