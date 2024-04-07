SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(triple arm-none-linux-gnueabihf)
SET(COMPILER_PREFIX ${triple}-)

SET(CMAKE_C_COMPILER ${COMPILER_PREFIX}gcc)
SET(CMAKE_C_COMPILER_TARGET ${triple})
SET(CMAKE_CXX_COMPILER ${COMPILER_PREFIX}g++)
SET(CMAKE_CXX_COMPILER_TARGET ${triple})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${ECB_PARAM_CPU}")
SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=${ECB_PARAM_CPU}")

MESSAGE(STATUS "Crosscompiling using ${COMPILER_PREFIX} and -mcpu=${ECB_PARAM_CPU}")

# it requires that libopenssl-dev:armhf is installed in debian
SET(OPENSSL_ROOT_DIR /usr/lib/${triple})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
