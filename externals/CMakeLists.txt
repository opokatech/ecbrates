add_library(mongoose STATIC mongoose/mongoose.c)
target_include_directories(mongoose PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mongoose)
