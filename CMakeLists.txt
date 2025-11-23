cmake_minimum_required(VERSION 3.28)
project(nodiflux LANGUAGES CXX)

file(GLOB SRC_FILES src/*.cpp)

# Lib
add_library(nodiflux STATIC ${SRC_FILES})
target_compile_features(nodiflux PRIVATE cxx_std_17)
target_include_directories(nodiflux PUBLIC ${PROJECT_SOURCE_DIR}/include PRIVATE ${PROJECT_SOURCE_DIR}/src)

# Example/test program
add_executable(test_main test/test_main.cpp)
target_link_libraries(test_main PRIVATE nodiflux)
target_include_directories(test_main PRIVATE ${PROJECT_SOURCE_DIR}/include)
