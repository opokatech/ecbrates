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
TARGET_LINK_LIBRARIES(ecb_lib ${EXTERNALS_LIBRARIES})
SET_PROPERTY(TARGET ecb_lib PROPERTY CXX_STANDARD 17)
