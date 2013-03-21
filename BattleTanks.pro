DESTDIR = bin

LIBS += -lSDL

QT += widgets sql multimedia xml

SOURCES += \
        main.cpp \
        silnik.cpp \
        ekran.cpp \
        plansza.cpp \
        logika.cpp \
        menu.cpp \
        obiekt.cpp \
        pojazd.cpp \
        animacja.cpp \
        pocisk.cpp \
        tekstura.cpp \
        specyfikacjapojazdu.cpp \
        specyfikacjaanimacji.cpp \
        pojazdgracza.cpp \
        pojazdobcy.cpp \
        specyfikacjapocisku.cpp \
    gamepad.cpp \
    klawiatura.cpp \
    dzwiek.cpp \
    pauza.cpp \
    widzety.cpp \
    funkcje.cpp \
    ladowanie.cpp \
    graf.cpp \
    wierzcholek.cpp \
    porownajsciezki.cpp \
    bazadanychsql.cpp \
    bazadanychxml.cpp \
    bazadanych.cpp

HEADERS += \
        silnik.h \
        ekran.h \
        plansza.h \
        logika.h \
        menu.h \
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
        specyfikacjapocisku.h \
    gamepad.h \
    klawiatura.h \
    dzwiek.h \
    pauza.h \
    widzety.h \
    funkcje.h \
    ladowanie.h \
    graf.h \
    porownajsciezki.h \
    wierzcholek.h \
    bazadanychsql.h \
    bazadanych.h \
    bazadanychxml.h

RC_FILE     = res.rc








