########################################
####
####    SimpleSocket Cmake compiler
####
########################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

## Tell CMake to use relative paths
CMAKE_POLICY(SET CMP0015 NEW)
set(CMAKE_BUILD_TYPE Release)

## Setup our project
PROJECT (SimpleSocket)

## Basic definitions for compilation
SET (CMAKE_CXX_FLAGS_DISTRIBUTION "${CMAKE_CXX_FLAGS} -Wall")
SET (CMAKE_C_FLAGS_DISTRIBUTION "-Wall -O3")

## Add C++11 standard support
include(CheckCXXCompilerFlag)

  ## Check to see if the compiler is c++11 Compliant
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

  ## Add the compilation flags when true
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. While SimpleSocket can be compiled without a new compiler, we reccomend updating ;)")
endif()

## Move the compiled library to ../lib
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

## Compile a shared library
ADD_LIBRARY(ssocket SHARED include/SSocket.cpp)

## Link SimpleSocket to curl 
TARGET_LINK_LIBRARIES (ssocket "-lcurl")