aux_source_directory(. c_example)

include_directories(${GTK2_INCLUDE_DIRS})

foreach(src ${c_example})
    get_filename_component(mainName ${src} NAME_WE)
    add_executable(${mainName} ${src})
    target_link_libraries(${mainName} ${GTK2_LIBRARIES})
endforeach(src)