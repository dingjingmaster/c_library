TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

HEADERS += \
        clib_log.h

SOURCES += \
        clib_log.c \
        clib_log_example.c
