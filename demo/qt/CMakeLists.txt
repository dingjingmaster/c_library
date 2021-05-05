FILE(GLOB QT_DEMO . *.cpp)

pkg_check_modules (QT5DEMO REQUIRED Qt5Core Qt5Network)
include_directories(${QT5DEMO_INCLUDE_DIRS})

foreach(src2 ${QT_DEMO})
    get_filename_component(mainName ${src2} NAME_WE)
    add_executable(${mainName} ${src2})
    target_link_libraries(${mainName} ${QT5DEMO_LIBRARIES})
    target_compile_options(${mainName} PRIVATE -fPIC)
endforeach(src2)