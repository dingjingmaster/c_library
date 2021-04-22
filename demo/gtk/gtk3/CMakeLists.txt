FILE(GLOB GTK3_DEMO . gtk3[_-]*.c)

foreach(src3 ${GTK3_DEMO})
    include_directories(${GTK3_INCLUDE_DIRS})
    get_filename_component(mainName ${src3} NAME_WE)
    add_executable(${mainName} ${src3})
    target_link_libraries(${mainName} ${GTK3_LIBRARIES})
endforeach(src3)