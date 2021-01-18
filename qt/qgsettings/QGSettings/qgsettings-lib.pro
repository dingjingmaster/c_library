TEMPLATE = lib
TARGET = gsettings-qt

QT -= gui
QT += core
CONFIG += qt no_keywords link_pkgconfig create_pc create_prl no_install_prl c++11

PKGCONFIG += gio-2.0

INCLUDEPATH += .

HEADERS = \
#    util.h\
#    conftypes.h\
    qconftype.h \
    qgsettings.h

SOURCES = \
#    util.cpp\
#    qconftypes.cpp\
    qconftype.cpp \
    qgsettings.cpp
