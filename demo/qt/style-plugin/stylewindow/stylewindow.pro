TARGET = styleplugin

QT += widgets

HEADERS = \
    PWD/style-window.h

SOURCES = \
    PWD/main.cpp \
    PWD/style-window.cpp

win32 {
    debug:DESTDIR = ../debug/
    release:DESTDIR = ../release/
} else {
    DESTDIR    = ../
}

# install
target.path = $$PWD/styleplugin/
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
