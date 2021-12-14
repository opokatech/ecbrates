# CXXOPTS - header only library, so we export the include directory here
set(CXXOPTS_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/cxxopts/include
    CACHE INTERNAL "")

# PUGIXML - it shuold export include libs automatically
add_subdirectory(pugixml)

# CPR
set(BUILD_CPR_TESTS
    OFF
    CACHE BOOL "")
set(BUILD_CURL_EXE
    OFF
    CACHE BOOL "" FORCE)
set(CURL_STATICLIB
    ON
    CACHE BOOL "" FORCE)
add_subdirectory(cpr)

# GTEST - are added if needs to be in ../CMakeLists.txt

# JSONCPP
set(JSONCPP_WITH_TESTS
    OFF
    CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_POST_BUILD_UNITTEST
    OFF
    CACHE INTERNAL "" FORCE)
set(JSONCPP_WITH_PKGCONFIG_SUPPORT
    OFF
    CACHE INTERNAL "" FORCE)
add_subdirectory(jsoncpp)

# MONGOOSE
add_subdirectory(mongoose)
set(MONGOOSE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/mongoose
    CACHE INTERNAL "")

# In main file we use those:
set(EXTERNALS_INCLUDE_DIRS
    ${CPR_INCLUDE_DIRS} ${MONGOOSE_INCLUDE_DIRS}
    CACHE INTERNAL "")

set(EXTERNALS_LIBRARIES
    ${CPR_LIBRARIES} pugixml jsoncpp_lib_static mongoose
    CACHE INTERNAL "")

message(STATUS "Using EXTERNALS_LIBRARIES: ${EXTERNALS_LIBRARIES}")
