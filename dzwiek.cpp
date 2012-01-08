#include "dzwiek.h"

#include <Phonon/AudioOutput>

int Dzwiek::glosnosc = 5;

Dzwiek::Dzwiek(const QString &nazwaPliku):
	QObject(),
	nazwaPliku(nazwaPliku)
{
	this->media = Phonon::createPlayer(Phonon::MusicCategory, nazwaPliku);
	Phonon::Path sciezka = this->media->outputPaths().first();
	Phonon::AudioOutput *wyjscie = static_cast<Phonon::AudioOutput*>(sciezka.sink());
	wyjscie->setVolume(this->glosnosc / 10.0);
	connect(this->media, SIGNAL(finished()), SIGNAL(zakonczony()));
}

Dzwiek::~Dzwiek()
{
	delete this->media;
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
