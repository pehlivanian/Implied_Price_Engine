cmake_minimum_required(VERSION 2.6)
project(Implied_Price_Engine_All)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=c++14 -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

# Handle Boost here?
# find_package(Boost COMPONENTS system filesystem REQUIRED)
# include_directories(${BOOST_INCLUDE_DIR})
# link_directories(${BOOST_LIBRARY_DIR})


add_subdirectory(include)
add_subdirectory(utils)
add_subdirectory(Timed_examples)
add_subdirectory(Quote_Server)
add_subdirectory(reg_test)
add_subdirectory(gtest)
