CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(ecbrates)

option(ECB_BUILD_TESTS "Build with tests" OFF)

# ==================================================================================================================== #

# default flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -pipe")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

SET(CMAKE_EXPORT_COMPILE_COMMANDS True)
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    ADD_COMPILE_DEFINITIONS(DEBUG)
ENDIF()

# ==================================================================================================================== #

ADD_SUBDIRECTORY(externals)

# ==================================================================================================================== #

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/src
    ${EXTERNALS_INCLUDE_DIRS}
    )

# ==================================================================================================================== #

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

# ==================================================================================================================== #

SET(ecb_sources

    src/Ecb.cpp
    src/Logging.cpp
    src/Timepoint.cpp
    src/Utils.cpp
    src/Server.cpp
    )

# ==================================================================================================================== #
# git stuff
FIND_PACKAGE(Git)

IF(GIT_EXECUTABLE)
    EXECUTE_PROCESS(COMMAND "${GIT_EXECUTABLE}" describe --always --abbrev=20 --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_HASH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    EXECUTE_PROCESS(COMMAND "${GIT_EXECUTABLE}" log -1 --format=%ad --date=iso
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF()

# ==================================================================================================================== #

ADD_LIBRARY(ecb_lib ${ecb_sources})
SET_PROPERTY(TARGET ecb_lib PROPERTY CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(ecb_lib ${EXTERNALS_LIBRARIES})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/Version.cpp.in ${CMAKE_CURRENT_BINARY_DIR}/Version.cpp)

ADD_EXECUTABLE(ecbrates src/main.cpp "${CMAKE_CURRENT_BINARY_DIR}/Version.cpp")
SET_PROPERTY(TARGET ecbrates PROPERTY CXX_STANDARD 17)
TARGET_LINK_LIBRARIES(ecbrates ecb_lib)

ADD_CUSTOM_TARGET(strip
    COMMAND ${COMPILER_PREFIX}strip ${CMAKE_BINARY_DIR}/bin/ecbrates
    DEPENDS ecbrates
    )

if (ECB_BUILD_TESTS)
    FILE(GLOB TEST_SOURCES tests/*.cpp)

    ADD_SUBDIRECTORY(externals/googletest)

    ADD_EXECUTABLE(ecbrates_test ${TEST_SOURCES})
    TARGET_LINK_LIBRARIES(ecbrates_test ecb_lib ${EXTERNALS_LIBRARIES} gtest_main)
    SET_PROPERTY(TARGET ecbrates_test PROPERTY CXX_STANDARD 17)
    ADD_CUSTOM_COMMAND(TARGET ecbrates_test POST_BUILD COMMAND ecbrates_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endif()
