#ifndef PLANSZA_H
#define PLANSZA_H

#include <QPixmap>
#include <QList>
#include "specyfikacjapojazdu.h"
#include "specyfikacjapocisku.h"
#include "specyfikacjaanimacji.h"
#include "animacja.h"

#define WYSOKOSC_WIDOKU 1080
#define MARGINES 400

class Ekran;
class PojazdGracza;
class PojazdObcy;
class Pocisk;

class Plansza{
	private:
		Ekran* ekran;
		QList<SpecyfikacjaPojazdu> specyfikacjePojazdow;
		QList<SpecyfikacjaPocisku> specyfikacjePociskow;
		QList<SpecyfikacjaAnimacji> specyfikacjeAnimacji;
		Tekstura* mapa;
		QPoint widok;
		QList<QPolygonF> przeszkody;
		PojazdGracza* pojazdGracza;
		Tekstura celownik;
		QList<PojazdObcy*> pojazdyObce;
		QList<Pocisk*> pociski;
		QList<Animacja*> animacje;
		QList<Animacja*> bonusy;
		
		void odswiezWidok();
		void rysujCelownik(QPainter&);
		
	public:
		Plansza(Ekran*);
		void dodajSpecyfikacje(const SpecyfikacjaPojazdu&);
		void dodajSpecyfikacje(const SpecyfikacjaPocisku&);
		void dodajSpecyfikacje(const SpecyfikacjaAnimacji&);
		bool zaladuj(QString);
		void czysc();
		void rysuj();
	
	friend class Logika;
};

#endif // PLANSZA_H
