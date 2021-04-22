FILE(GLOB GTK2_DEMO . gtk2[_-]*.c)

foreach(src2 ${GTK2_DEMO})
    include_directories(${GTK2_INCLUDE_DIRS})
    get_filename_component(mainName ${src2} NAME_WE)
    add_executable(${mainName} ${src2})
    target_link_libraries(${mainName} ${GTK2_LIBRARIES})
endforeach(src2)
