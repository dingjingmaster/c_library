aux_source_directory(. cxx_example)
include_directories(${GLIB_INCLUDE_DIRS})

foreach(src ${cxx_example})
    get_filename_component(mainName ${src} NAME_WE)
    add_executable(${mainName} ${src})
    target_link_libraries(${mainName} ${LIB_CORE_DINGJING_NAME} Qt5::Core ${GLIB})
endforeach(src)
