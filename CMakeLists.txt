cmake_minimum_required(VERSION 3.16)

project(onegin)
set(SOURSES oneggin.cpp)
set(HEADERS oneggin.h)
add_library(Onegin ${SOURSES} ${HEADERS})
target_include_directories(Onegin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})