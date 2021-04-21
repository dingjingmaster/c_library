aux_source_directory(. c_example)

foreach(src ${c_example})
    get_filename_component(mainName ${src} NAME_WE)
    message("building ${src} --- target ${mainName}")
    add_executable(${mainName} ${src})
    target_link_libraries(${mainName} ${LIB_CORE_DINGJING_NAME})
endforeach(src)
