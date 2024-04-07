message(STATUS "Building with tests")

add_executable(ecb_tests Rates_Tests.cpp Record_Tests.cpp Time_Point_Tests.cpp Utils_Tests.cpp)
target_link_libraries(ecb_tests ecb_lib gtest_main)

add_test(NAME ecb_tests COMMAND ecb_tests)
