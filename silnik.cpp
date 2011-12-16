#include "silnik.h"
#include "urzadzeniewejscia.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"
#include "menu.h"
#include "logika.h"

Silnik::Silnik(){
	this->urzadzenieWejscia = new UrzadzenieWejscia;
	this->ekran = new Ekran;
	this->bazaDanych = new BazaDanych;
	this->plansza = new Plansza(this->ekran, 1080, 300);
	this->menu = new Menu(this->ekran, this->bazaDanych, this->plansza);
	this->logika = new Logika(this->plansza);
	
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(odswiez()));
	this->urzadzenieWejscia->otworz(0);
	this->bazaDanych->polacz();
	
	this->ekran->ustawRozdzielczosc(QSize(500, 500));
	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(this->plansza->wysokoscWidoku());
	this->zaladujSpecyfikecjeObiektow();
	
	//	dopiac kontrole uruchomienia poszczegolnych elementow
}

Silnik::~Silnik(){
	delete this->urzadzenieWejscia;
	delete this->ekran;
	delete this->bazaDanych;
	delete this->plansza;
	delete this->menu;
	delete this->logika;
}

void Silnik::zaladujSpecyfikecjeObiektow(){
	//	dodac specyfikacje obiektow do planszy
	
	//	DEMO
	
	QString nazwaPojazdu = "pojazdTestowy";
	this->plansza->dodajSpecyfikacje(
		SpecyfikacjaPojazdu(
			QPixmap(nazwaPojazdu + "Korpus.png"),	//	teksturaKorpus
			QPixmap(nazwaPojazdu + "Wieza.png"),	//	teksturaWieza
			20,		//	przesuniecieOsiDlaKorpusu
			40,		//	przesuniecieOsiDlaWiezy
			200,	//	predkoscMaksymalnaPojazdu
			2.0,	//	predkoscRotacjiWiezy
			100		//	wytrzymalosc
		)
	);
	
	QString nazwaAnimacji = "animacjaTestowa";
	this->plansza->dodajSpecyfikacje(
		SpecyfikacjaAnimacji(
			QPixmap(nazwaAnimacji + ".png"),	//	tekstury
			QSize(4, 4)	//	iloscKlatek
		)
	);
}

void Silnik::odswiezMenu(int milisekundy){
	Menu::Akcja akcja;
	
	if(urzadzenieWejscia->statusNawigatorWcisniecie(0) & UrzadzenieWejscia::GORA)
		akcja = Menu::GORA;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie(0) & UrzadzenieWejscia::DOL)
		akcja = Menu::DOL;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie(0) & UrzadzenieWejscia::PRAWO)
		akcja = Menu::PRAWO;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie(0) & UrzadzenieWejscia::LEWO)
		akcja = Menu::LEWO;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(1))
		akcja = Menu::WYBIERZ;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(3))
		akcja = Menu::COFNIJ;
	else
		akcja = Menu::BRAK;
	
	this->tryb = this->menu->odswiez(milisekundy, akcja);
	this->menu->rysuj();
}

void Silnik::odswiezRozgrywke(int milisekundy){
	float predkoscGasienicyLewej = -this->urzadzenieWejscia->statusDzojstik(1);
	float predkoscGasienicyPrawej = -this->urzadzenieWejscia->statusDzojstik(3);
	
	int rotacjaWiezy = 0;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie(0) & UrzadzenieWejscia::LEWO || this->urzadzenieWejscia->statusPrzyciskPolozenie(6))
		rotacjaWiezy++;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie(0) & UrzadzenieWejscia::PRAWO || this->urzadzenieWejscia->statusPrzyciskPolozenie(7))
		rotacjaWiezy--;
	
	int zmianaZasiegu = 0;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie(0) & UrzadzenieWejscia::GORA)
		zmianaZasiegu++;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie(0) & UrzadzenieWejscia::DOL)
		zmianaZasiegu--;
	
	int zmianaBroni = 0;
	if(
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(2) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(4)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(4) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(2))
	)
		zmianaBroni++;
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(0))
		zmianaBroni--;
	
	bool wystrzal = (
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(1) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(5)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(5) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(1))
	);
	
	this->logika->odswiez(milisekundy, predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaZasiegu, zmianaBroni, wystrzal);
	
	//	sprawdzic czy koniec gry
	
	this->plansza->rysuj();
}

void Silnik::odswiez(){
	QTime czasAktualny = QTime::currentTime();
	int milisekundy = this->czasOstatniegoOdswiezenia.msecsTo(czasAktualny);
	
	if(milisekundy == 0)
		return;
	
	this->urzadzenieWejscia->odswiez();
	switch(this->tryb){
		case MENU:
			this->odswiezMenu(milisekundy);
			break;
		case ROZGRYWKA:
			this->odswiezRozgrywke(milisekundy);
			break;
	}
	
	this->czasOstatniegoOdswiezenia = czasAktualny;
}

void Silnik::uruchom(){
	this->tryb = MENU;
	this->czasOstatniegoOdswiezenia = QTime::currentTime();
	this->timer.start();
	this->ekran->show();
}
