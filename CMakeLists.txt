cmake_minimum_required(VERSION 3.7)
project(osxtools)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pipe -Wall -g -fPIC -O3")

set(SOURCE_FILES shuf/shuf.cc)

add_executable(shuf ${SOURCE_FILES})
