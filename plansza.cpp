#include "plansza.h"
#include "ekran.h"

Plansza::Plansza(Ekran* ekran, int widokWysokosc):
	ekran(ekran),
	widokWysokosc(widokWysokosc),
	mapa(0),
	pojazdGracza(0){
}

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

void Plansza::rysuj() const{
	//	rysowanie na ekranie :P
	
	this->ekran->update();
}
