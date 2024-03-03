include(FetchContent)

# OPTIONS
FetchContent_Declare(
    options
    GIT_REPOSITORY https://github.com/opokatech/options
    GIT_HASH c5a01ab
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching options...${ColorReset}")
FetchContent_MakeAvailable(options)
message(STATUS "${ColorGreen}options ready${ColorReset}")

# PUGIXML
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml
    GIT_TAG v1.14
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching pugixml...${ColorReset}")
FetchContent_MakeAvailable(pugixml)
message(STATUS "${ColorGreen}pugixml ready${ColorReset}")

# GOOGLE TEST
if(ECB_PARAM_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.14.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
    message(STATUS "${ColorYellow}Fetching googletest...${ColorReset}")
    FetchContent_MakeAvailable(googletest)
    message(STATUS "${ColorGreen}googletest ready${ColorReset}")
endif()
