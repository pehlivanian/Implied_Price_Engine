cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -std=c++14 -pedantic")
set(warnings "-Wall -Wparenthesis -Wno-unused-variable -Wno-sign-compare ")

include_directories("." "../include")

set(SOURCES Create_QuoteStream)

FOREACH(source ${SOURCES})
    add_executable(${source} ${source}.cpp)
    target_link_libraries(${source} LINK_PUBLIC IElib pthread boost_thread boost_system)
ENDFOREACH(source)

