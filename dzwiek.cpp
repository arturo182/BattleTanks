#include "dzwiek.h"

Dzwiek::Dzwiek(const QString &nazwaPliku):
	QObject(),
	nazwaPliku(nazwaPliku)
{
	this->media = Phonon::createPlayer(Phonon::MusicCategory, nazwaPliku);
	connect(this->media, SIGNAL(finished()), SIGNAL(zakonczony()));
}

void Dzwiek::odtworz()
{
	this->media->play();
}

void Dzwiek::odtworz(const QString &nazwaPliku)
{
	Dzwiek *dz = new Dzwiek(nazwaPliku);
	connect(dz, SIGNAL(zakonczony()), dz, SLOT(deleteLater()));

	dz->odtworz();
}
