add_library(ecb_lib STATIC Ecb.cpp Logging.cpp Record.cpp Time_Point.cpp Utils.cpp)
target_include_directories(ecb_lib PUBLIC .)
target_link_libraries(ecb_lib PRIVATE ecb_compiler_flags)

add_library(ecb_data_loader STATIC Data_Loader.cpp)
target_include_directories(ecb_data_loader PUBLIC .)
target_link_libraries(ecb_data_loader PRIVATE ecb_compiler_flags ecb_lib pugixml cpr::cpr)

# I added dl and pthread because of the static linking: If linked statically the order of libraries does matter and
# libcrypto.a (which is needed by curl which is needed by cpr) needs libdl and libpthread, so they must be added AFTER
# libcrypto. If linked dynamically it does not matter. target_link_libraries(ecb_lib PUBLIC jsoncpp_lib_static PRIVATE
# ${EXTERNALS_LIBRARIES} dl pthread) set_property(TARGET ecb_lib PROPERTY CXX_STANDARD 17)

add_executable(ecb_server Main_Server.cpp)
target_link_libraries(ecb_server PRIVATE ecb_compiler_flags options version_info ecb_lib)

add_custom_target(strip COMMAND ${COMPILER_PREFIX}strip ${CMAKE_BINARY_DIR}/bin/ecb_server DEPENDS ecb_server)

add_executable(ecb_fetcher Main_Fetcher.cpp)
target_link_libraries(ecb_fetcher PRIVATE ecb_compiler_flags options version_info ecb_lib ecb_data_loader)
