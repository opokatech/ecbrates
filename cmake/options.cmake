# Those settings should be set from outside
option(ECB_PARAM_DEBUG "Enables or disables debug" OFF)
option(ECB_PARAM_TESTS "Enables or disables tests" OFF)

if(ECB_PARAM_DEBUG)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
    message("${BoldBlue}Debug  : ${Green}ON${ColorReset} (CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})")
else()
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
    message("${BoldBlue}Debug  : ${Red}OFF${ColorReset} (CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})")
endif()

if(ECB_PARAM_TESTS)
    message("${BoldBlue}Tests  : ${Green}ON${ColorReset}")
else()
    message("${BoldBlue}Tests  : ${Red}OFF${ColorReset}")
endif()
