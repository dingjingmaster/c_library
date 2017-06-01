TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    clib_string/clib_string_test.c \
    clib_string/clib_string.c \
    clib_list/clib_list.c

HEADERS += \
    clib_string/clib_string.h \
    clib_list/clib_list.h
