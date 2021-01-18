TEMPLATE = app
TARGET = qgsettings-demo

QT += core gui
CONFIG += no_keywords link_pkgconfig c++11
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PKGCONFIG += gio-2.0

HEADERS = \
    $$PWD/QGSettings/qconftype.h \
    $$PWD/QGSettings/qgsettings.h \
    testkey.h

SOURCES = \
    $$PWD/main.cpp \
    $$PWD/QGSettings/qconftype.cpp \
    $$PWD/QGSettings/qgsettings.cpp \
    testkey.cpp
