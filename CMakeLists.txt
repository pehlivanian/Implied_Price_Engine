cmake_minimum_required(VERSION 2.6)
project(Implied_Price_Engine_All)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

add_subdirectory(include)
# add_subdirectory(reg_test)
add_subdirectory(gtest)
