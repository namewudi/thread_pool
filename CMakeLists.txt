cmake_minimum_required(VERSION 3.0.0)
project(threadpool VERSION 0.1.0)

include(CTest)
enable_testing()
SET(CMAKE_CXX_COMPILER "g++") 

add_executable(threadpool src/main.cpp include/threadpool.h include/threadpoolVT.h)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
