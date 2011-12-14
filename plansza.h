#ifndef PLANSZA_H
#define PLANSZA_H

#include <QPixmap>
#include <QList>
#include "specyfikacjapojazdu.h"
#include "specyfikacjaanimacji.h"
#include "pojazd.h"
#include "pocisk.h"
#include "animacja.h"

class Ekran;

class Plansza{
	private:
		Ekran* ekran;
		const int widokWysokosc;
		QList<SpecyfikacjaPojazdu> specyfikacjePojazdow;
		QList<SpecyfikacjaAnimacji> specyfikacjeAnimacji;
		Tekstura* mapa;
		//	przeszkody
		Pojazd* pojazdGracza;
		QList<Pojazd> pojazdyObce;
		QList<Pocisk> pociski;
		QList<Animacja> animacje;
		QList<Animacja> bonusy;
		
	public:
		Plansza(Ekran*, int);
		void dodajSpecyfikacje(const SpecyfikacjaPojazdu&);
		void dodajSpecyfikacje(const SpecyfikacjaAnimacji&);
		bool zaladuj(QString);
		void czysc();
		void rysuj() const;
};

#endif // PLANSZA_H
