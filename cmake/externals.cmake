include(FetchContent)

FetchContent_Declare(
    options
    GIT_REPOSITORY https://github.com/opokatech/options
    GIT_HASH c5a01ab
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching options...${ColorReset}")
FetchContent_MakeAvailable(options)
message(STATUS "${ColorGreen}options ready${ColorReset}")

if(ECB_PARAM_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
    message(STATUS "${ColorYellow}Fetching Google Test...${ColorReset}")
    FetchContent_MakeAvailable(googletest)
    message(STATUS "${ColorGreen}Google Test ready${ColorReset}")
endif()
