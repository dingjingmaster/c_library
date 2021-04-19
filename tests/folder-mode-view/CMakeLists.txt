add_executable("test-folder-model-view"
        folder-mode-view.cpp
        )

set (DEMO_LIBRARYS Qt5::Core Qt5::Widgets ${LIB_CORE_DINGJING_NAME})

target_link_libraries("test-folder-model-view" ${DEMO_LIBRARYS})
