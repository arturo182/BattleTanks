QT       += core gui

TARGET = EdytorMap
DESTDIR = ../bin
TEMPLATE = app

SOURCES += main.cpp\
    scena.cpp \
    przeszkoda.cpp \
    mainwindow.cpp \
    waypoint.cpp \
    sciezka.cpp

HEADERS  += \
    scena.h \
    przeszkoda.h \
    mainwindow.h \
    waypoint.h \
    sciezka.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    res.qrc

RC_FILE     = res.rc
