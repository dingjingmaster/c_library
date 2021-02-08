QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 link_pkgconfig no_keywords

INCLUDEPATH += \
        /usr/include/aarch64-linux-gnu/qt5/ \ 
        /usr/include/aarch64-linux-gnu/qt5/QGSettings 

PKGCONFIG += peony

SOURCES += \
    main.cpp \
    dialog.cpp \
    navigation-bar.cpp \
    path-bar.cpp \
    side-bar.cpp \
    tool-bar.cpp

HEADERS += \
    dialog.h \
    navigation-bar.h \
    path-bar.h \
    side-bar.h \
    tool-bar.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
