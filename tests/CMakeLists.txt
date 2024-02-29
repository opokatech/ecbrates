message(STATUS "Building with tests")

add_executable(ecbrates_test Time_Point_Tests.cpp)
target_link_libraries(ecbrates_test ecb_lib gtest_main)

enable_testing()
add_test(NAME ecb_rates_test COMMAND ecbrates_test)
