#include "plansza.h"
#include "ekran.h"
#include "pojazdgracza.h"
#include "pojazdobcy.h"
#include "pocisk.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>

Plansza::Plansza(Ekran* ekran, int widokWysokosc, int margines):
	ekran(ekran),
	widokWysokosc(widokWysokosc),
	margines(margines),
	mapa(0),
	pojazdGracza(0){}

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
	QPixmap mapaTekstura("dane/plansze/" + nazwaPlanszy + ".png");

	if(mapaTekstura.isNull())
		return false;
	this->mapa = new Tekstura(mapaTekstura);
	this->widok = Obiekt::skala * QPoint(0, 0);

	QFile mapaSpecyfikacjaPlik("dane/plansze/" + nazwaPlanszy + ".dat");
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

		QPolygon przeszkoda(iloscWierzcholkow);
		for(int j = 0; j < iloscWierzcholkow; j++){
			mapaSpecyfikacjaDane >> wierzcholek;
			przeszkoda.setPoint(j, wierzcholek);
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


	//minimapa
	int szerokoscEkranu = this->ekran->buforObrazu.width();
	int wysokoscEkranu = this->ekran->buforObrazu.height();
	float skala = (szerokoscEkranu * 0.15) / this->mapa->teksturaOryginalna.width();

	painter.setPen(QPen(Qt::black, 2.0));
	painter.setBrush(QColor(255, 255, 255, 128));
	painter.drawRect(QRectF(szerokoscEkranu * 0.85 - wysokoscEkranu * 0.05, wysokoscEkranu * 0.05, szerokoscEkranu * 0.15, szerokoscEkranu * 0.15));

	painter.setBrush(Qt::black);
	painter.translate(szerokoscEkranu * 0.85 - wysokoscEkranu * 0.05, wysokoscEkranu * 0.05);
	painter.scale(skala, skala);

	foreach(QPolygon poly, this->przeszkody)
		painter.drawPolygon(poly);

	painter.resetTransform();

	painter.setBrush(Qt::red);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(QPointF(szerokoscEkranu * 0.85 - wysokoscEkranu * 0.05, wysokoscEkranu * 0.05) +  QPointF(this->pojazdGracza->pozycja.x() * skala, this->pojazdGracza->pozycja.y() * skala), szerokoscEkranu * 0.003, szerokoscEkranu * 0.003);

	painter.end();
	this->ekran->update();
}

void Plansza::odswiezWidok(){
	int margines = Obiekt::skala * this->margines;

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
