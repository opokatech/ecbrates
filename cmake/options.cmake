# Those settings should be set from outside
option(ECB_PARAM_DEBUG "Enables or disables debug" OFF)
option(ECB_PARAM_SAN "Enables or disables sanitizers" OFF)
option(ECB_PARAM_TESTS "Enables or disables tests" OFF)

if(ECB_PARAM_DEBUG)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
    message("${BoldBlue}Debug     : ${Green}ON${ColorReset} (CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})")
else()
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
    message("${BoldBlue}Debug     : ${Red}OFF${ColorReset} (CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})")
endif()

if(ECB_PARAM_SAN)
    message("${BoldBlue}Sanitizers: ${Green}ON${ColorReset}")
else()
    message("${BoldBlue}Sanitizers: ${Red}OFF${ColorReset}")
endif()

if(ECB_PARAM_TESTS)
    message("${BoldBlue}Tests     : ${Green}ON${ColorReset}")
else()
    message("${BoldBlue}Tests     : ${Red}OFF${ColorReset}")
endif()
