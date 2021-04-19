FILE (GLOB ALL_QRC "core/*.qrc")
FILE (GLOB ALL_FORMS "core/*.ui")
FILE (GLOB ALL_SOURCES
        "job.cpp"
        "utils.cpp"
        "folder.cpp"
        "mime-type.cpp"
        "file-info.cpp"
        "file-path.cpp"
        "icon-info.cpp"
        "fm-config.cpp"
        "thumbnailer.cpp"
        "dir-list-job.cpp"
        "thumbnail-job.cpp"
        "file-info-job.cpp"
        "volume-manager.cpp"
        "dnd-action-menu.cpp"
        "x-dnd-workaround.cpp"
        "filesystem-info-job.cpp"

        "folder-model-view/folder-view.cpp"
        "folder-model-view/folder-model.cpp"
        "folder-model-view/folder-model-item.cpp"
        "folder-model-view/proxy-folder-model.cpp"
        "folder-model-view/cached-folder-model.cpp"
        "folder-model-view/folder-item-delegate.cpp"

        "pathbar-model-view/path-editor.cpp"
        )

FILE (GLOB ALL_HEADERS
        "job.h"
        "job_p.h"
        "utils.h"
        "global.h"
        "folder.h"
        "mime-type.h"
        "file-info.h"
        "file-path.h"
        "icon-info.h"
        "fm-config.h"
        "file-info-p.h"
        "icon-info-p.h"
        "smart-ptr.hpp"
        "thumbnailer.h"
        "dir-list-job.h"
        "thumbnail-job.h"
        "file-info-job.h"
        "volume-manager.h"
        "dnd-action-menu.h"
        "x-dnd-workaround.h"
        "filesystem-info-job.h"

        "folder-model-view/folder-view.h"
        "folder-model-view/folder-model.h"
        "folder-model-view/folder-view-p.h"
        "folder-model-view/folder-model-item.h"
        "folder-model-view/proxy-folder-model.h"
        "folder-model-view/cached-folder-model.h"
        "folder-model-view/folder-item-delegate.h"

        "pathbar-model-view/path-editor.h"
        "pathbar-model-view/path-editor-p.h"
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
