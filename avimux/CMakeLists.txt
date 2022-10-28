# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(avi)

include_directories(
        /usr/include/gstreamer-1.0 
        /usr/include/glib-2.0 
        /usr/lib/x86_64-linux-gnu/glib-2.0/include 
        )

# message(WARNING ${CMAKE_MFC_FLAG})
set (CMAKE_CXX_STANDARD 14)
add_compile_options(${CMAKE_MFC_FLAG} )
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}  -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 )
