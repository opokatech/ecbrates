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

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/Version.cpp.in ${CMAKE_BINARY_DIR}/Version.cpp)

ADD_EXECUTABLE(ecbrates src/Main.cpp ${CMAKE_BINARY_DIR}/Version.cpp)
TARGET_INCLUDE_DIRECTORIES(ecbrates PRIVATE src ${CXXOPTS_INCLUDE_DIRS} ${MONGOOSE_INCLUDE_DIRS} src)
TARGET_LINK_LIBRARIES(ecbrates PRIVATE ecb_lib)
SET_PROPERTY(TARGET ecbrates PROPERTY CXX_STANDARD 17)

ADD_CUSTOM_TARGET(strip
    COMMAND ${COMPILER_PREFIX}strip ${CMAKE_BINARY_DIR}/bin/ecbrates
    DEPENDS ecbrates
    )

if (ECB_BUILD_TESTS)
    FILE(GLOB TEST_SOURCES tests/*.cpp)

    ADD_SUBDIRECTORY(externals/googletest)

    ADD_EXECUTABLE(ecbrates_test ${TEST_SOURCES})
    TARGET_LINK_LIBRARIES(ecbrates_test ecb_lib ${EXTERNALS_LIBRARIES} gtest_main)
    TARGET_INCLUDE_DIRECTORIES(ecbrates_test PRIVATE src)
    SET_PROPERTY(TARGET ecbrates_test PROPERTY CXX_STANDARD 17)

    ADD_CUSTOM_COMMAND(TARGET ecbrates_test POST_BUILD COMMAND ecbrates_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endif()
