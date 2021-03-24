project(libcore-dingjing)
cmake_minimum_required(VERSION 3.15)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR YES)


add_definitions("-Wall")


set(QT_MINIMUM_VERSION "5.12.0")
set(GLIB_MINIMUM_VERSION "2.50.0")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


find_package(PkgConfig)
find_package(Qt5Widgets "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5X11Extras "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5LinguistTools "${QT_MINIMUM_VERSION}" REQUIRED)

pkg_check_modules (GLIB REQUIRED glib-2.0 gio-2.0 gobject-2.0 gthread-2.0)
pkg_check_modules (XCB REQUIRED xcb)


message(STATUS "Building ${PROJECT_NAME} with Qt ${Qt5Core_VERSION}")


add_subdirectory(core)


add_library(core-dingjing STATIC ${ALL_SOURCES})


#target_link_libraries(core-dingjing
#        Qt5::Widgets
#        Qt5::X11Extras
#)


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