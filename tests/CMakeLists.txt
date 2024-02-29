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
