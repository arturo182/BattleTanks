QT       += core gui sql

TARGET = EdytorMap
DESTDIR = ../bin
TEMPLATE = app

SOURCES += main.cpp\
    scena.cpp \
    przeszkoda.cpp \
    waypoint.cpp \
    sciezka.cpp \
    oknospecyfikacji.cpp \
    oknoglowne.cpp \
    gracz.cpp \
    oknopojazduobcego.cpp \
    meta.cpp

HEADERS  += \
    scena.h \
    przeszkoda.h \
    waypoint.h \
    sciezka.h \
    oknospecyfikacji.h \
    oknoglowne.h \
    gracz.h \
    oknopojazduobcego.h \
    meta.h

FORMS    += \
    oknospecyfikacji.ui \
    oknoglowne.ui \
    oknopojazduobcego.ui

RESOURCES += \
    res.qrc

RC_FILE     = res.rc
