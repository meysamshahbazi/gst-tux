# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(hello)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTLIBS REQUIRED
        gobject-2.0
        glib-2.0
        gstreamer-webrtc-1.0
        gstreamer-sdp-1.0
        gstreamer-pbutils-1.0
        # libsoup-2.4
        # json-glib-1.0
        # gstreamer-rtsp-server-1.0
        gstreamer-check-1.0)

# Declare the executable target built from your sources
include_directories(
        /usr/include/gstreamer-1.0 
        /usr/include/glib-2.0 
        /usr/lib/x86_64-linux-gnu/glib-2.0/include 
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        /usr/include/glib-2.0
        /usr/local/include/gstreamer-1.0
        # /usr/local/include/libsoup-2.4
        # /usr/local/include/json-glib-1.0
    )

# message(WARNING ${CMAKE_MFC_FLAG})
set (CMAKE_CXX_STANDARD 14)
add_compile_options(${CMAKE_MFC_FLAG} )
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME}  -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 )
