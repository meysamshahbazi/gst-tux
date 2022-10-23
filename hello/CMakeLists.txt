# cmake needs this line
cmake_minimum_required(VERSION 3.1)

find_package(PkgConfig REQUIRED)
# Define project name
project(hello)


# Declare the executable target built from your sources

set (CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} main.cpp)

