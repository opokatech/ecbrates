# source files
SET(ECB_SOURCES

    Ecb.cpp
    Logging.cpp
    Server.cpp
    Timepoint.cpp
    Utils.cpp
    )

ADD_LIBRARY(ecb_lib ${ECB_SOURCES})
TARGET_INCLUDE_DIRECTORIES(ecb_lib PRIVATE ${EXTERNALS_INCLUDE_DIRS})
# I added dl and pthread because of the static linking:
# If linked statically the order of libraries does matter and libcrypto.a (which is needed by curl which is needed by
# cpr) needs libdl and libpthread, so they must be added AFTER libcrypto.
# If linked dynamically it does not matter.
TARGET_LINK_LIBRARIES(ecb_lib ${EXTERNALS_LIBRARIES} dl pthread)
SET_PROPERTY(TARGET ecb_lib PROPERTY CXX_STANDARD 17)
