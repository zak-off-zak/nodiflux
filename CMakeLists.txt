cmake_minimum_required(VERSION 3.28)
project(nodiflux LANGUAGES CXX)

file(GLOB SRC_FILES src/*.cpp)

# Library
add_library(nodiflux STATIC ${SRC_FILES})
target_compile_features(nodiflux PRIVATE cxx_std_17)
target_include_directories(nodiflux PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Test executable
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_main test/test_main.cpp)
target_link_libraries(test_main PRIVATE nodiflux gtest_main)
add_test(NAME MainTests COMMAND test_main)

add_executable(test_packet test/test_packet.cpp)
target_link_libraries(test_packet PRIVATE nodiflux gtest_main)
add_test(NAME PacketTests COMMAND test_packet)
