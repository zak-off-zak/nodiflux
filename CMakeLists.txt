cmake_minimum_required(VERSION 3.28)
project(nodiflux LANGUAGES CXX)

file(GLOB SRC_FILES src/*.cpp)

# Library
add_library(nodiflux STATIC ${SRC_FILES})
target_compile_features(nodiflux PRIVATE cxx_std_17)
target_include_directories(nodiflux PUBLIC ${PROJECT_SOURCE_DIR}/include)
