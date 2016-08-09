cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -Werror")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(memcached_sample memcached_sample.c)
target_link_libraries(memcached_sample -lmemcached)
