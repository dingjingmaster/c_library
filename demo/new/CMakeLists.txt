cmake_minimum_required(VERSION 3.15)
project(new C)
set(CMAKE_C_STANDARD 99)

find_package(PkgConfig)
pkg_search_module(GOBJECT2 REQUIRED gobject-2.0)
pkg_search_module(GTK3 REQUIRED gtk+-3.0)

MESSAGE(STATUS ${GOBJECT2_LIBRARIES})
MESSAGE(STATUS ${GOBJECT2_INCLUDE_DIRS})
MESSAGE(STATUS ${GTK3_LIBRARIES})
MESSAGE(STATUS ${GTK3_INCLUDE_DIRS})

link_libraries(${GOBJECT2_LIBRARIES})
link_libraries(${GTK3_LIBRARIES})
link_libraries(X11)

include_directories(${GOBJECT2_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})

#add_executable(new main.c)
add_executable(new xlib1.c)