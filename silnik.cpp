#include "silnik.h"
#include <QDebug>	//	DEBUG
#include <windows.h>	//	WINDOWS.H

Silnik::Silnik(){
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(odswiez()));
}

void Silnik::odswiezMenu(int milisekundy){
	qDebug() << "MENU" << milisekundy;
	Sleep(100);
}

void Silnik::odswiezRozgrywke(int milisekundy){
	qDebug() << "ROZGRYWKA" << milisekundy;
	Sleep(100);
}

void Silnik::odswiez(){
	int milisekundy = this->czas.restart();
	//	co bedzie gdy milisekundy == 0 ?
	
	switch(this->tryb){
		case MENU:
			this->odswiezMenu(milisekundy);
			break;
		case ROZGRYWKA:
			this->odswiezRozgrywke(milisekundy);
			break;
	}
}

void Silnik::uruchom(){
	this->tryb = MENU;
	this->czas.start();
	this->timer.start();
}
