DESTDIR = bin

LIBS += \
    -lSDL

QT += sql
QT += sql phonon

SOURCES += \
    main.cpp \
    silnik.cpp \
    ekran.cpp \
    plansza.cpp \
    logika.cpp \
    menu.cpp \
    bazadanych.cpp \
    obiekt.cpp \
    pojazd.cpp \
    animacja.cpp \
    pocisk.cpp \
    urzadzeniewejscia.cpp \
    tekstura.cpp \
    specyfikacjapojazdu.cpp \
    specyfikacjaanimacji.cpp \
    pojazdgracza.cpp \
    pojazdobcy.cpp \
    specyfikacjapocisku.cpp

HEADERS += \
    silnik.h \
    ekran.h \
    plansza.h \
    logika.h \
    menu.h \
    bazadanych.h \
    obiekt.h \
    pojazd.h \
    animacja.h \
    pocisk.h \
    urzadzeniewejscia.h \
    tekstura.h \
    specyfikacjapojazdu.h \
    specyfikacjaanimacji.h \
    pojazdgracza.h \
    pojazdobcy.h \
    specyfikacjapocisku.h





