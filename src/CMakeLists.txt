
# add_library(ecb_lib STATIC Ecb.cpp Logging.cpp Server.cpp Timepoint.cpp Utils.cpp)
# target_include_directories(ecb_lib PRIVATE ${EXTERNALS_INCLUDE_DIRS})
# target_include_directories(ecb_lib PUBLIC .)

# I added dl and pthread because of the static linking: If linked statically the order of libraries
# does matter and libcrypto.a (which is needed by curl which is needed by cpr) needs libdl and
# libpthread, so they must be added AFTER libcrypto. If linked dynamically it does not matter.
# target_link_libraries(ecb_lib PUBLIC jsoncpp_lib_static PRIVATE ${EXTERNALS_LIBRARIES} dl pthread)
# set_property(TARGET ecb_lib PROPERTY CXX_STANDARD 17)

add_executable(ecbrates Main.cpp)
target_link_libraries(ecbrates PRIVATE options version_info)

# add_custom_target(
#     strip
#     COMMAND ${COMPILER_PREFIX}strip ${CMAKE_BINARY_DIR}/bin/ecbrates
#     DEPENDS ecbrates)
