cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} "../include")

#Link runTests with what we want to test and GTest and pthread libraries
add_executable(SecPair_gtest SecPair_gtest.cpp)
target_link_libraries(SecPair_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} pthread)
# add_executable(ImpliedEngine_gtest ImpliedEngine_gtest.cpp)
# target_link_libraries(ImpliedEngine_gtest LINK_PUBLIC IElib ${GTEST_LIBRARIES} pthread)
