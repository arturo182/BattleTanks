#ifndef SILNIK_H
#define SILNIK_H

#include <QTime>
#include <QTimer>
#include "urzadzeniewejscia.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"
#include "menu.h"
#include "logika.h"
#include "tryb.h"

class Silnik: public QObject{
	Q_OBJECT
	
	private:
		UrzadzenieWejscia urzadzenieWejscia;
		Ekran ekran;
		BazaDanych bazaDanych;
		Plansza plansza;
		Menu menu;
		Logika logika;
		
		Tryb tryb;
		QTimer timer;
		QTime czasOstatniegoOdswiezenia;
		
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		
	private slots:
		void odswiez();
		
	public:
		Silnik();
		void uruchom();
};

#endif // SILNIK_H
