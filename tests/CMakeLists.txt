project(subprojects)


include_directories(${GLIB_INCLUDE_DIRS} ${XCB_INCLUDE_DIRS} ${EXIF_INCLUDE_DIRS})

add_subdirectory(c)
add_subdirectory(core)
add_subdirectory(folder-mode-view)
#add_subdirectory(pathbar-mode-view)
