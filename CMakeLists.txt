CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(ecbrates)

# ==================================================================================================================== #
OPTION(ECB_BUILD_TESTS "Build with tests" OFF)

# ==================================================================================================================== #

# default flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -pipe")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXPORT_COMPILE_COMMANDS True)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_COMPILE_DEFINITIONS(DEBUG)
ENDIF()

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

ADD_SUBDIRECTORY(externals)
ADD_SUBDIRECTORY(src)

if (ECB_BUILD_TESTS)
    ADD_SUBDIRECTORY(tests)
endif()
