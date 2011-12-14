#include "silnik.h"
#include <QDebug>		//	DEBUG
#include <windows.h>	//	WINDOWS.H

Silnik::Silnik():
	plansza(this->ekran, 1080),
	menu(this->ekran, this->bazaDanych, this->plansza),
	logika(this->plansza){
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(odswiez()));
	
	//	dopiac kontrole uruchomienia poszczegolnych elementow
	this->urzadzenieWejscia.otworz(0);
	this->bazaDanych.polacz();
}

void Silnik::odswiezMenu(int milisekundy){
	//		odswiezenie trybu "MENU"
	//	nalezy odczytac dane z urzadzenia wejsciowego
	//	nastepnie wywolac metody klasy menu zgodnie z jej interejsem
	//	nalezy umozliwic zmiane trybu pracy silnika
	
	qDebug() << "MENU" << milisekundy;
	
	if(this->urzadzenieWejscia.statusPrzyciskWcisniecie(9))
		this->tryb = ROZGRYWKA;
}

void Silnik::odswiezRozgrywke(int milisekundy){
	double predkoscGasienicyLewej = -this->urzadzenieWejscia.statusDzojstik(1);
	double predkoscGasienicyPrawej = -this->urzadzenieWejscia.statusDzojstik(3);
	
	int rotacjaWiezy = 0;
	if(this->urzadzenieWejscia.statusNawigatorPolozenie(0) & LEWO || this->urzadzenieWejscia.statusPrzyciskPolozenie(6))
		rotacjaWiezy++;
	if(this->urzadzenieWejscia.statusNawigatorPolozenie(0) & PRAWO || this->urzadzenieWejscia.statusPrzyciskPolozenie(7))
		rotacjaWiezy--;
	
	int zmianaZasiegu = 0;
	if(this->urzadzenieWejscia.statusNawigatorPolozenie(0) & GORA)
		zmianaZasiegu++;
	if(this->urzadzenieWejscia.statusNawigatorPolozenie(0) & DOL)
		zmianaZasiegu--;
	
	int zmianaBroni = 0;
	if(
		(this->urzadzenieWejscia.statusPrzyciskWcisniecie(2) && !this->urzadzenieWejscia.statusPrzyciskPolozenie(4)) ||
		(this->urzadzenieWejscia.statusPrzyciskWcisniecie(4) && !this->urzadzenieWejscia.statusPrzyciskPolozenie(2))
	)
		zmianaBroni++;
	if(this->urzadzenieWejscia.statusPrzyciskWcisniecie(0))
		zmianaBroni--;
	
	bool wystrzal = (
		(this->urzadzenieWejscia.statusPrzyciskWcisniecie(1) && !this->urzadzenieWejscia.statusPrzyciskPolozenie(5)) ||
		(this->urzadzenieWejscia.statusPrzyciskWcisniecie(5) && !this->urzadzenieWejscia.statusPrzyciskPolozenie(1))
	);
	
	this->logika.odswiez(predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaZasiegu, zmianaBroni, wystrzal);
	
	//	sprawdzic czy koniec gry
	//	odswiezyc obraz
}

void Silnik::odswiez(){
	QTime czasAktualny = QTime::currentTime();
	int milisekundy = this->czasOstatniegoOdswiezenia.msecsTo(czasAktualny);
	
	if(milisekundy == 0)
		return;
	
	this->urzadzenieWejscia.odswiez();
	switch(this->tryb){
		case MENU:
			this->odswiezMenu(milisekundy);
			break;
		case ROZGRYWKA:
			this->odswiezRozgrywke(milisekundy);
			break;
	}
	
	this->czasOstatniegoOdswiezenia = czasAktualny;
	
	Sleep(100);	//	SLEEP
}

void Silnik::uruchom(){
	this->tryb = MENU;
	this->czasOstatniegoOdswiezenia = QTime::currentTime();
	this->timer.start();
}
