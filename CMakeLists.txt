project(subprojects)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR YES)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

set(LIBRARY_NAME "libcore-dingjing")
ADD_DEFINITIONS("-DQT_NO_KEYWORDS" "-Wall" "-g")

set(QT_MINIMUM_VERSION "5.12.0")
set(GLIB_MINIMUM_VERSION "2.50.0")
set(LIB_CORE_DINGJING_NAME "core-dingjing" CACHE STRING "core-dingjing")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


find_package(PkgConfig)
find_package(Qt5Core "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5Widgets "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5X11Extras "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5LinguistTools "${QT_MINIMUM_VERSION}" REQUIRED)

find_library(PTHREAD pthread REQUIRED)

pkg_check_modules (XCB REQUIRED xcb)
pkg_check_modules (EXIF REQUIRED libexif)
pkg_check_modules (VTE REQUIRED vte-2.91)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)
pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
pkg_check_modules (OPENSSL REQUIRED openssl)
pkg_check_modules (QT5BASE REQUIRED Qt5Core Qt5Widgets Qt5Network)
pkg_check_modules (GLIB REQUIRED glib-2.0 gio-2.0 gio-unix-2.0 gobject-2.0 gthread-2.0)

message(STATUS "include glib -- ${GLIB_INCLUDE_DIRS}")
message(STATUS "include xcb  -- ${XCB_INCLUDE_DIRS}")
message(STATUS "include exif -- ${EXIF_INCLUDE_DIRS}")

include_directories(${CMAKE_SOURCE_DIR}/core/)

message(STATUS "Building ${PROJECT_NAME} with Qt ${Qt5Core_VERSION}")

add_subdirectory(core)
#add_subdirectory(demo)
add_subdirectory(tests)


option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    configure_file("${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" @ONLY)
    add_custom_target(doc ALL
            ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
    install(DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}/doc"
            DESTINATION "${CMAKE_INSTALL_DOCDIR}"
            COMPONENT Devel
            )
endif()
