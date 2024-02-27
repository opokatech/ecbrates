cmake_minimum_required(VERSION 3.16)
project(ecbrates VERSION 2.0.0)

option(ECB_BUILD_TESTS "Build with tests" OFF)
option(ECB_STATIC_BUILD "Use static linking" OFF)

# default flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -pipe")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(ECB_STATIC_BUILD)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    message(STATUS "Building statically")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

add_subdirectory(externals)
add_subdirectory(src)

# git stuff
find_package(Git)

if(GIT_EXECUTABLE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --always --abbrev=20 --dirty
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_HASH
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" log -1 --format=%ad --date=iso
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DATE
        ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

configure_file(${CMAKE_SOURCE_DIR}/src/Version.cpp.in ${CMAKE_BINARY_DIR}/Version.cpp)

add_executable(ecbrates src/Main.cpp ${CMAKE_BINARY_DIR}/Version.cpp)
# TODO should be removed and include dirs should be provide by those libs
target_include_directories(ecbrates PRIVATE ${MONGOOSE_INCLUDE_DIRS})
target_link_libraries(ecbrates PRIVATE jsoncpp_lib_static ecb_lib cxxopts)
set_property(TARGET ecbrates PROPERTY CXX_STANDARD 17)

add_custom_target(
    strip
    COMMAND ${COMPILER_PREFIX}strip ${CMAKE_BINARY_DIR}/bin/ecbrates
    DEPENDS ecbrates)

if(ECB_BUILD_TESTS)
    message(STATUS "Building with tests")
    file(GLOB TEST_SOURCES tests/*.cpp)

    add_subdirectory(externals/googletest)

    add_executable(ecbrates_test ${TEST_SOURCES})
    target_link_libraries(ecbrates_test ecb_lib ${EXTERNALS_LIBRARIES} gtest_main)
    target_include_directories(ecbrates_test PRIVATE src) # TODO remove
    set_property(TARGET ecbrates_test PROPERTY CXX_STANDARD 17)

    add_custom_command(
        TARGET ecbrates_test
        POST_BUILD
        COMMAND ecbrates_test
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endif()
