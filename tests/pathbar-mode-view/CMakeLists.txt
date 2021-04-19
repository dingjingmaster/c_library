add_executable("test-pathbar-model-view" pathbar-demo.cpp)

set (DEMO_LIBRARYS Qt5::Core Qt5::Widgets ${LIB_CORE_DINGJING_NAME})

target_link_libraries("test-pathbar-model-view" ${DEMO_LIBRARYS})

