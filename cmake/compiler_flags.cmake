# Explicitly set some defaults for some of the CMAKE_BUILD_TYPE
set(CMAKE_CXX_FLAGS_DEBUG "-g -fstack-protector-all")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

add_library(ecb_compiler_flags INTERFACE)
target_compile_options(
    ecb_compiler_flags
    INTERFACE -Wall
              -Wextra
              -pedantic
              -Wundef
              -Wuninitialized
              -Wno-psabi
              -Wshadow
              -pipe
              $<$<BOOL:${ECB_PARAM_SAN}>:-fsanitize=address -fsanitize=leak -fsanitize=undefined>
              $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor> # for C++ only
)

if(ECB_PARAM_SAN)
    target_link_libraries(ecb_compiler_flags INTERFACE asan ubsan)
endif()
