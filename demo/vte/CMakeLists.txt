aux_source_directory(. example)

include_directories(${VTE_INCLUDE_DIRS} ${GTK3_INCLUDE_DIRS})
foreach(src ${example})
    get_filename_component(mainName ${src} NAME_WE)
    add_executable(${mainName} ${src})
    target_link_libraries(${mainName} ${GLIB_LIBRARIES} ${GTK3_LIBRARIES} ${VTE_LIBRARIES} X11 xcb)
endforeach(src)