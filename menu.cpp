#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza):
	ekran(ekran),
	bazaDanych(bazaDanych),
	plansza(plansza){
	this->zaladujSpecyfikecjeObiektow();
}

Silnik::Tryb Menu::odswiez(Akcja akcja){
	//	obsluga akcji
	
	if(akcja == WYBIERZ)
		return Silnik::ROZGRYWKA;
	else
		return Silnik::MENU;
}

void Menu::rysuj() const{
	//	rysowanie na ekranie :P
	
	//	TEST
	
	QPainter painter(&this->ekran->buforObrazu);
	painter.setBrush(Qt::red);
	painter.drawRect(QRect(5, 5, 5, 5));
	painter.end();
	
	this->ekran->update();
}

void Menu::zaladujSpecyfikecjeObiektow(){
	//	dodac specyfikacje obiektow do planszy
	
	
	//	DEMO
	
	QString nazwaPojazdu = "pojazdTestowy";
	this->plansza->dodajSpecyfikacje(
		SpecyfikacjaPojazdu(
			QPixmap(nazwaPojazdu + "Korpus.png"),	//	teksturaKorpus
			QPixmap(nazwaPojazdu + "Wieza.png"),	//	teksturaWieza
			50,		//	przesuniecieWiezy
			100,	//	predkoscMaksymalna
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
