include(FetchContent)

# OPTIONS
FetchContent_Declare(
    options
    GIT_REPOSITORY https://github.com/opokatech/options
    GIT_TAG v3.0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching options...${ColorReset}")
FetchContent_MakeAvailable(options)
message(STATUS "${ColorGreen}options ready${ColorReset}")

# PUGIXML
FetchContent_Declare(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml
    GIT_TAG v1.15
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching pugixml...${ColorReset}")
FetchContent_MakeAvailable(pugixml)
message(STATUS "${ColorGreen}pugixml ready${ColorReset}")

# CPR
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/whoshuu/cpr
    GIT_TAG 1.11.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
set(BUILD_CPR_TESTS OFF CACHE BOOL "")
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(CURL_STATICLIB ON CACHE BOOL "" FORCE)
message(STATUS "${ColorYellow}Fetching cpr...${ColorReset}")
FetchContent_MakeAvailable(cpr)
message(STATUS "${ColorGreen}cpr ready${ColorReset}")

# JSONCPP
FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp
    GIT_TAG 1.9.6
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
set(JSONCPP_WITH_TESTS OFF CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE INTERNAL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE INTERNAL "" FORCE)
message(STATUS "${ColorYellow}Fetching jsoncpp...${ColorReset}")
FetchContent_MakeAvailable(jsoncpp)
message(STATUS "${ColorGreen}jsoncpp ready${ColorReset}")

# MONGOOSE
FetchContent_Declare(
    mongoose
    GIT_REPOSITORY https://github.com/cesanta/mongoose
    GIT_TAG 7.15
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)
message(STATUS "${ColorYellow}Fetching mongoose...${ColorReset}")
FetchContent_MakeAvailable(mongoose)
message(STATUS "${ColorGreen}mongoose ready${ColorReset}")
add_library(mongoose STATIC ${mongoose_SOURCE_DIR}/mongoose.c)
target_include_directories(mongoose PUBLIC ${mongoose_SOURCE_DIR})

# SPDLOG
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE)

message(STATUS "${ColorYellow}Making spdlog available...${ColorReset}")
FetchContent_MakeAvailable(spdlog)
message(STATUS "${ColorGreen}spdlog ready${ColorReset}")

# GOOGLE TEST
if(ECB_PARAM_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.16.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
    message(STATUS "${ColorYellow}Fetching googletest...${ColorReset}")
    FetchContent_MakeAvailable(googletest)
    message(STATUS "${ColorGreen}googletest ready${ColorReset}")
endif()
