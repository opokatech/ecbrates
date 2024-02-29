# Build date
string(TIMESTAMP BUILD_DATE UTC)

find_package(Git)

if(GIT_FOUND)
    # Hash
    execute_process(
        COMMAND git rev-parse --verify HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Date iso8601 format e.g. 2019-07-24T17:55:34+05:30
    execute_process(
        COMMAND git log -1 --date=iso8601-strict --pretty=format:%cd
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_DATE
        OUTPUT_STRIP_TRAILING_WHITESPACE)
else()
    set(GIT_HASH "Unknown")
    set(GIT_DATE "Unknown")
endif()

configure_file(${PROJECT_SOURCE_DIR}/src/Version_Info.cpp.in ${CMAKE_BINARY_DIR}/Version_Info.cpp)
add_library(version_info STATIC ${CMAKE_BINARY_DIR}/Version_Info.cpp)
target_include_directories(version_info PUBLIC ${PROJECT_SOURCE_DIR}/src)
