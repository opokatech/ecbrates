add_library(mongoose STATIC mongoose/mongoose.c)
target_include_directories(mongoose PUBLIC mongoose)
