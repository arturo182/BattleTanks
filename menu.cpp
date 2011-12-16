#include "menu.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

Menu::Menu(Ekran* ekran, BazaDanych* bazaDanych, Plansza* plansza):
	ekran(ekran),
	bazaDanych(bazaDanych),
	plansza(plansza){}

Silnik::Tryb Menu::odswiez(int milisekundy, Akcja akcja){
	//	obsluga akcji
	
	if(akcja == WYBIERZ){
		this->plansza->zaladuj("planszaTestowa");
		return Silnik::ROZGRYWKA;
	}else
		return Silnik::MENU;
}

void Menu::rysuj() const{
	//	rysowanie na ekranie :P
	
	//	TEST
	
	QPainter painter(&this->ekran->buforObrazu);
	painter.fillRect(this->ekran->buforObrazu.rect(), Qt::black);
	painter.setPen(Qt::red);
	painter.setBrush(Qt::red);
	painter.drawRect(QRect(5, 5, 5, 5));
	painter.end();
	
	this->ekran->update();
}
