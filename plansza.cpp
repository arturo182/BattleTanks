#include "plansza.h"
#include "ekran.h"

Plansza::Plansza(Ekran* ekran, int widokWysokosc, int margines):
	ekran(ekran),
	widokWysokosc(widokWysokosc),
	margines(margines),
	mapa(0),
	pojazdGracza(0){}

void Plansza::dodajSpecyfikacje(const SpecyfikacjaPojazdu& specyfikacja){
	this->specyfikacjePojazdow.append(specyfikacja);
}

void Plansza::dodajSpecyfikacje(const SpecyfikacjaAnimacji& specyfikacja){
	this->specyfikacjeAnimacji.append(specyfikacja);
}

bool Plansza::zaladuj(QString nazwaPlanszy){
	QPixmap mapa(nazwaPlanszy + ".png");
	
	if(mapa.isNull())
		return false;
	this->mapa = new Tekstura(mapa);
	
	this->widok = Obiekt::skala * QPoint(0, 0);
	this->pojazdGracza = new Pojazd(&this->specyfikacjePojazdow[0], QPointF(100, 100), 0);
	
	return true;
}

void Plansza::czysc(){
	delete this->mapa;
	this->mapa = 0;
	delete this->pojazdGracza;
	this->pojazdGracza = 0;
	this->pojazdyObce.clear();
	this->pociski.clear();
	this->animacje.clear();
	this->bonusy.clear();
}

void Plansza::rysuj(){
	QPainter painter(&this->ekran->buforObrazu);
	
	this->odswiezWidok();
	
	painter.drawPixmap(
		QPoint(0, 0),
		this->mapa->teksturaPrzeskalowana,
		QRect(this->widok, this->ekran->buforObrazu.size())
	);
	
	this->pojazdGracza->rysuj(painter, this->widok);
	
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
