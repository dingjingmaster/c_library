project(libcore-dingjing)
cmake_minimum_required(VERSION 3.15)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR YES)


ADD_DEFINITIONS("-DQT_NO_KEYWORDS" "-Wall")


set(QT_MINIMUM_VERSION "5.12.0")
set(GLIB_MINIMUM_VERSION "2.50.0")
set(LIB_CORE_DINGJING_NAME "core-dingjing" CACHE STRING "core-dingjing")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)


find_package(PkgConfig)
find_package(Qt5Widgets "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5X11Extras "${QT_MINIMUM_VERSION}" REQUIRED)
find_package(Qt5LinguistTools "${QT_MINIMUM_VERSION}" REQUIRED)

pkg_check_modules (XCB REQUIRED xcb)
pkg_check_modules (EXIF REQUIRED libexif)
pkg_check_modules (GLIB REQUIRED glib-2.0 gio-2.0 gobject-2.0 gthread-2.0)

include_directories(${GLIB_INCLUDE_DIRS} ${XCB_INCLUDE_DIRS} ${EXIF_INCLUDE_DIRS})

message(STATUS "Building ${PROJECT_NAME} with Qt ${Qt5Core_VERSION}")

FILE (GLOB ALL_QRC "core/*.qrc")
FILE (GLOB ALL_FORMS "core/*.ui")
FILE (GLOB ALL_SOURCES
        "core/job.cpp"
        "core/utils.cpp"
        "core/folder.cpp"
        "core/mime-type.cpp"
        "core/file-info.cpp"
        "core/file-path.cpp"
        "core/icon-info.cpp"
        "core/fm-config.cpp"
        "core/thumbnailer.cpp"
        "core/dir-list-job.cpp"
        "core/thumbnail-job.cpp"
        "core/file-info-job.cpp"
        "core/volume-manager.cpp"
        "core/dnd-action-menu.cpp"
        "core/x-dnd-workaround.cpp"
        "core/filesystem-info-job.cpp"

        "folder-model-view/folder-view.cpp"
        "folder-model-view/folder-model.cpp"
        "folder-model-view/folder-model-item.cpp"
        "folder-model-view/proxy-folder-model.cpp"
        "folder-model-view/cached-folder-model.cpp"
        "folder-model-view/folder-item-delegate.cpp"
)

FILE (GLOB ALL_HEADERS
        "core/job.h"
        "core/job_p.h"
        "core/utils.h"
        "core/global.h"
        "core/folder.h"
        "core/mime-type.h"
        "core/file-info.h"
        "core/file-path.h"
        "core/icon-info.h"
        "core/fm-config.h"
        "core/file-info-p.h"
        "core/icon-info-p.h"
        "core/smart-ptr.hpp"
        "core/thumbnailer.h"
        "core/dir-list-job.h"
        "core/thumbnail-job.h"
        "core/file-info-job.h"
        "core/volume-manager.h"
        "core/dnd-action-menu.h"
        "core/x-dnd-workaround.h"
        "core/filesystem-info-job.h"

        "folder-model-view/folder-view.h"
        "folder-model-view/folder-model.h"
        "folder-model-view/folder-view-p.h"
        "folder-model-view/folder-model-item.h"
        "folder-model-view/proxy-folder-model.h"
        "folder-model-view/cached-folder-model.h"
        "folder-model-view/folder-item-delegate.h"
)

# 调用预编译器moc，需要使用 QT5_WRAP_CPP宏
QT5_WRAP_CPP(ALL_HEADERS_MOC ${ALL_HEADERS})

# 使用uic处理.ui文件
QT5_WRAP_UI(ALL_FORMS_HEADERS ${ALL_FORMS})

# 使用rcc处理.qrc文件
QT5_ADD_RESOURCES(ALL_RESOURCES_RCC ${ALL_QRC})

# 这些生成的中间文件都会在build目录下，这样的话，编译器则不能定位由uic程序产生的诸如_ui_mainwindow.h等文件。
# 所以，我们需要把build目录添加到包含目录中
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} PRIVATE "${Qt5Gui_PRIVATE_INCLUDE_DIRS}")

add_library(${LIB_CORE_DINGJING_NAME} STATIC ${ALL_SOURCES} ${ALL_HEADERS_MOC} ${ALL_RESOURCES_RCC} ${ALL_FORMS_HEADERS})


target_link_libraries(${LIB_CORE_DINGJING_NAME} Qt5::Widgets Qt5::X11Extras ${GLIB_LIBRARIES} ${XCB_LIBRARIES} ${EXIF_LIBRARIES})

# demo library
set (DEMO_LIBRARYS Qt5::Core Qt5::Widgets ${LIB_CORE_DINGJING_NAME})

# demo
add_executable("test-folder-model-view"
    demo/folder-mode-view/folder-mode-view.cpp
)
target_link_libraries("test-folder-model-view" ${DEMO_LIBRARYS})


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
