symbian:TARGET.UID3 = 0xE29B9FE0
symbian:TARGET.CAPABILITY = WriteUserData ReadUserData UserEnvironment
symbian:TARGET.EPOCSTACKSIZE = 0x24000
symbian:TARGET.EPOCHEAPSIZE = 0x6400000 0x10000000

QT += sql #phonon

symbian {
baza.source = bin/baza.db
pomoc.source = bin/pomoc
grafika.source = bin/grafika
plansze.source = bin/plansze
DEPLOYMENTFOLDERS = baza pomoc grafika plansze
}

CONFIG += mobility
MOBILITY += sensors multimedia

SOURCES += main.cpp \
    wierzcholek.cpp \
    widzety.cpp \
    tekstura.cpp \
    specyfikacjapojazdu.cpp \
    specyfikacjapocisku.cpp \
    specyfikacjaanimacji.cpp \
    silnik.cpp \
    porownajsciezki.cpp \
    pojazdobcy.cpp \
    pojazdgracza.cpp \
    pojazd.cpp \
    pocisk.cpp \
    plansza.cpp \
    pauza.cpp \
    obiekt.cpp \
    menu.cpp \
    logika.cpp \
    ladowanie.cpp \
    klawiatura.cpp \
    graf.cpp \
    funkcje.cpp \
    ekran.cpp \
    dzwiek.cpp \
    bazadanych.cpp \
    animacja.cpp \
    dotyk.cpp

HEADERS += \
    wierzcholek.h \
    widzety.h \
    urzadzeniewejscia.h \
    tekstura.h \
    specyfikacjapojazdu.h \
    specyfikacjapocisku.h \
    specyfikacjaanimacji.h \
    silnik.h \
    porownajsciezki.h \
    pojazdobcy.h \
    pojazdgracza.h \
    pojazd.h \
    pocisk.h \
    plansza.h \
    pauza.h \
    obiekt.h \
    menu.h \
    logika.h \
    ladowanie.h \
    klawiatura.h \
    graf.h \
    funkcje.h \
    ekran.h \
    dzwiek.h \
    bazadanych.h \
    animacja.h \
    dotyk.h

FORMS +=

include(deployment.pri)
qtcAddDeployment()
