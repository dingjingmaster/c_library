#   一定要注意 子目录的 .pro 文件名字和目录名字相同才可以
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    clib_list.c \
    clib_list_test.c

HEADERS += \
    clib_list.h

DISTFILES += \
    readme.md
