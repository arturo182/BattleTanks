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
		const int margines;
		QList<SpecyfikacjaPojazdu> specyfikacjePojazdow;
		QList<SpecyfikacjaAnimacji> specyfikacjeAnimacji;
		Tekstura* mapa;
		QPoint widok;
		//	przeszkody
		Pojazd* pojazdGracza;
		QList<Pojazd> pojazdyObce;
		QList<Pocisk> pociski;
		QList<Animacja> animacje;
		QList<Animacja> bonusy;
		
		void odswiezWidok();
		
	public:
		Plansza(Ekran*, int, int);
		inline int wysokoscWidoku() const;
		void dodajSpecyfikacje(const SpecyfikacjaPojazdu&);
		void dodajSpecyfikacje(const SpecyfikacjaAnimacji&);
		bool zaladuj(QString);
		void czysc();
		void rysuj();
	
	friend class Logika;
};

inline int Plansza::wysokoscWidoku() const{
	return this->widokWysokosc;
}

#endif // PLANSZA_H
