#ifndef SILNIK_H
#define SILNIK_H

#include <QTime>
#include <QTimer>
#include "ekran.h"
#include "menu.h"
#include "plansza.h"
#include "logika.h"
#include "bazadanych.h"

class Silnik: public QObject{
	Q_OBJECT
	
	private:
		Ekran ekran;
		Menu menu;
		Plansza plansza;
		Logika logika;
		BazaDanych bazaDanych;
		QTime czas;
		QTimer timer;
		enum{
			MENU, ROZGRYWKA
		} tryb;
		
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		
	private slots:
		void odswiez();
		
	public:
		Silnik();
		void uruchom();
};

#endif // SILNIK_H
