project(utility)
cmake_minimum_required(VERSION 2.8)

file(GLOB Srcs ./*.cc)

add_executable(main ${Srcs})
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")
