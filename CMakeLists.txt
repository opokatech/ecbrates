CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(ecbrates)

option(ECB_BUILD_TESTS "Build with tests" OFF)

# ==================================================================================================================== #

# default flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -pipe")
SET(CMAKE_CXX_STANDARD 17)
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

ADD_LIBRARY(ecb_lib ${ecb_sources})
TARGET_LINK_LIBRARIES(ecb_lib ${EXTERNALS_LIBRARIES})

ADD_EXECUTABLE(ecbrates src/main.cpp)
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
	ADD_CUSTOM_COMMAND(TARGET ecbrates_test POST_BUILD COMMAND ecbrates_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endif()
