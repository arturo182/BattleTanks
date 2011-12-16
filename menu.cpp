#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza):
	ekran(ekran),
	bazaDanych(bazaDanych),
	plansza(plansza){
	this->tryb = TEST1;
}

Silnik::Tryb Menu::odswiez(int milisekundy, Akcja akcja){
	//	obsluga akcji
	
	if(this->tryb == TEST3){
		this->plansza->zaladuj("planszaTestowa");
		return Silnik::ROZGRYWKA;
	}else if(this->tryb == TEST2){
		if(akcja == WYBIERZ)
			this->tryb = TEST3;
		else if(akcja == COFNIJ)
			this->tryb = TEST1;
	}else if(this->tryb == TEST1 && akcja == WYBIERZ)
		this->tryb = TEST2;
		
	return Silnik::MENU;
}

void Menu::rysuj() const{
	//	rysowanie na ekranie :P
	
	//	TEST
	
	QPainter painter(&this->ekran->buforObrazu);
	painter.fillRect(this->ekran->buforObrazu.rect(), Qt::black);
	painter.setPen(Qt::green);
	
	if(this->tryb == TEST1)
		painter.drawText(10, 10, "Aby rozpoczac gre wcisnij przycisk 2");
	else if(this->tryb == TEST2)
		painter.drawText(10, 10, "Jestec pewien ? 2 - TAK | 4 - COFNIJ");
	else if(this->tryb == TEST3)
		painter.drawText(10, 10, "Ladowanie...");
	
	painter.end();
	
	this->ekran->update();
}
