QT       += core gui

TARGET = EdytorMap
DESTDIR = ../bin
TEMPLATE = app

SOURCES += main.cpp\
           MainWindow.cpp \
    Przeszkoda.cpp

HEADERS  += MainWindow.h \
    Przeszkoda.h \
    Scena.h

FORMS    += MainWindow.ui

RESOURCES += \
    res.qrc

RC_FILE     = res.rc
