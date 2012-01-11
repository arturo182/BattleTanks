#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "plansza.h"
#include "ekran.h"
#include "pojazdgracza.h"
#include "pojazdobcy.h"
#include "pocisk.h"

Plansza::Plansza(Ekran* ekran):
	ekran(ekran),
	mapa(0),
	pojazdGracza(0),
	celownik(QPixmap("grafika/celownik.png")){}

void Plansza::dodajSpecyfikacje(const SpecyfikacjaPojazdu& specyfikacja){
	this->specyfikacjePojazdow.append(specyfikacja);
}

void Plansza::dodajSpecyfikacje(const SpecyfikacjaPocisku& specyfikacja){
	this->specyfikacjePociskow.append(specyfikacja);
}

void Plansza::dodajSpecyfikacje(const SpecyfikacjaAnimacji& specyfikacja){
	this->specyfikacjeAnimacji.append(specyfikacja);
}

bool Plansza::zaladuj(QString nazwaPlanszy){
	QPixmap mapaTekstura("plansze/" + nazwaPlanszy + ".png");

	if(mapaTekstura.isNull())
		return false;
	this->mapa = new Tekstura(mapaTekstura);
	this->widok = Obiekt::skala * QPoint(0, 0);
	
	QFile mapaSpecyfikacjaPlik("plansze/" + nazwaPlanszy + ".dat");
	if(!mapaSpecyfikacjaPlik.open(QIODevice::ReadOnly)){
		delete this->mapa;
		return false;
	}
	QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);

	//	typ mapy

	int iloscPrzeszkod, iloscWierzcholkow;
	QPoint wierzcholek;

	mapaSpecyfikacjaDane >> iloscPrzeszkod;
	for(int i = 0; i < iloscPrzeszkod; i++){
		mapaSpecyfikacjaDane >> iloscWierzcholkow;

		QPolygonF przeszkoda;
		for(int j = 0; j < iloscWierzcholkow; j++){
			mapaSpecyfikacjaDane >> wierzcholek;
			przeszkoda << wierzcholek;
		}
		this->przeszkody.append(przeszkoda);
	}
	
	this->pojazdGracza = new PojazdGracza(&this->specyfikacjePojazdow[0], QPointF(100, 100), 0.0, this->specyfikacjePociskow.size(), 0);

	mapaSpecyfikacjaPlik.close();
	return true;
}

void Plansza::czysc(){
	delete this->mapa;
	this->mapa = 0;

	delete this->pojazdGracza;
	this->pojazdGracza = 0;

	this->przeszkody.clear();

	qDeleteAll(this->pojazdyObce);
	this->pojazdyObce.clear();

	qDeleteAll(this->pociski);
	this->pociski.clear();

	qDeleteAll(this->animacje);
	this->animacje.clear();

	qDeleteAll(this->bonusy);
	this->bonusy.clear();
}

void Plansza::rysuj(){
	if(!this->mapa || !this->pojazdGracza)
		return;
	QPainter painter(&this->ekran->buforObrazu);

	this->odswiezWidok();

	painter.drawPixmap(
		QPoint(0, 0),
		this->mapa->teksturaPrzeskalowana,
		QRect(this->widok, this->ekran->buforObrazu.size())
	);

	this->pojazdGracza->rysuj(painter, this->widok);

	for(QList<Animacja*>::iterator i = this->animacje.begin(); i != this->animacje.end(); i++)
		(*i)->rysuj(painter, this->widok);

	for(QList<Pocisk*>::iterator i = this->pociski.begin(); i != this->pociski.end(); i++)
		(*i)->rysuj(painter, this->widok);
	
	this->rysujCelownik(painter);
	
	//	DEBUG
	painter.setPen(Qt::red);
	painter.setBrush(QColor(0, 255, 0, 128));
	for(QList<QPolygonF>::iterator i = this->przeszkody.begin(); i != this->przeszkody.end(); i++)
		painter.drawPolygon(i->translated(-widok));

	painter.end();
	this->ekran->update();
}

void Plansza::rysujCelownik(QPainter& painter){
	painter.drawPixmap(
		Obiekt::skala * (this->pojazdGracza->punktWylotuLufy() + this->pojazdGracza->celownikOdleglosc * this->pojazdGracza->zwrotWiezyWektor.toPointF()) - this->widok - QPointF(this->celownik.teksturaPrzeskalowana.width() / 2.0, this->celownik.teksturaPrzeskalowana.height() / 2.0),
		this->celownik.teksturaPrzeskalowana
	);
}

void Plansza::odswiezWidok(){
	int margines = Obiekt::skala * MARGINES;

	if(Obiekt::skala * this->pojazdGracza->pozycja.x() - this->widok.x() < margines){
		if(Obiekt::skala * this->pojazdGracza->pozycja.x() > margines)
			this->widok.setX(Obiekt::skala * this->pojazdGracza->pozycja.x() - margines);
		else
			this->widok.setX(0);
	}else if(this->widok.x() + this->ekran->buforObrazu.width() - Obiekt::skala * this->pojazdGracza->pozycja.x() < margines){
		if(this->mapa->teksturaPrzeskalowana.width() - Obiekt::skala * this->pojazdGracza->pozycja.x() > margines)
			this->widok.setX(Obiekt::skala * this->pojazdGracza->pozycja.x() + margines - this->ekran->buforObrazu.width());
		else
			this->widok.setX(this->mapa->teksturaPrzeskalowana.width() - this->ekran->buforObrazu.width());
	}

	if(Obiekt::skala * this->pojazdGracza->pozycja.y() - this->widok.y() < margines){
		if(Obiekt::skala * this->pojazdGracza->pozycja.y() > margines)
			this->widok.setY(Obiekt::skala * this->pojazdGracza->pozycja.y() - margines);
		else
			this->widok.setY(0);
	}else if(this->widok.y() + this->ekran->buforObrazu.height() - Obiekt::skala * this->pojazdGracza->pozycja.y() < margines){
		if(this->mapa->teksturaPrzeskalowana.height() - Obiekt::skala * this->pojazdGracza->pozycja.y() > margines)
			this->widok.setY(Obiekt::skala * this->pojazdGracza->pozycja.y() + margines - this->ekran->buforObrazu.height());
		else
			this->widok.setY(this->mapa->teksturaPrzeskalowana.height() - this->ekran->buforObrazu.height());
	}
}
