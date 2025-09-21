cmake_minimum_required(VERSION 3.30)
project(spirva VERSION 1.0)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(${CMAKE_PROJECT_NAME} src/test.c)