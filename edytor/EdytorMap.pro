QT       += core gui

TARGET = EdytorMap
DESTDIR = ../bin
TEMPLATE = app

SOURCES += main.cpp\
           MainWindow.cpp \
    Przeszkoda.cpp

HEADERS  += MainWindow.h \
    Przeszkoda.h

FORMS    += MainWindow.ui

RESOURCES += \
    res.qrc
