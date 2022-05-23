cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(constexpr_things)

add_executable(things)
target_sources(things PRIVATE src/constexpr.cpp)

