add_executable("test-folder-model-view"
        folder-mode-view.cpp
        )
include_directories(${QT5BASE_INCLUDE_DIRS})

target_link_libraries("test-folder-model-view" ${LIB_CORE_DINGJING_NAME} Qt5::Core Qt5::Widgets)
