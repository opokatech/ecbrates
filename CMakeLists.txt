cmake_minimum_required(VERSION 3.19) # presets are available since 3.19
project(ecbrates VERSION 2.0.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/colors.cmake)
include(cmake/options.cmake)
include(cmake/version_info.cmake)
include(cmake/compiler_flags.cmake)
include(cmake/externals.cmake) # add external projects via FetchContent

add_subdirectory(src)

if(ECB_PARAM_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
