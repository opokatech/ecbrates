# Explicitly set some defaults for some of the CMAKE_BUILD_TYPE
set(CMAKE_CXX_FLAGS_DEBUG "-g -fstack-protector-all")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")

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
              $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor> # for C++ only
)
