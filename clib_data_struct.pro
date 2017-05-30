TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    clib_string/clit_string.c

HEADERS += \
    clib_string/clib_string.h
