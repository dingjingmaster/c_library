cmake_minimum_required(VERSION 3.15)

add_definitions("-Wall")
add_definitions("-std=gnu99")


set(QT_MINIMUM_VERSION "5.12.0")
set(GLIB_MINIMUM_VERSION "2.50.0")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


find_package(PkgConfig)
find_package(Qt5Widgets "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5X11Extras "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5LinguistTools "${QT_MINIMUM_VERSION}" REQUIRED)

pkg_check_modules (GLIB REQUIRED glib-2.0 gio-2.0 gobject-2.0 gthread-2.0)
pkg_check_modules (XCB REQUIRED xcb)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR YES)


# shared library
#set(DLIB "sharelib")
#aux_source_directory("." DLIB_SRC)
#add_library(${DLIB} SHARED ${DLIB_SRC})