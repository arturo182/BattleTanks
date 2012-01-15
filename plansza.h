#ifndef PLANSZA_H
#define PLANSZA_H

#include <QPixmap>
#include <QList>
#include "specyfikacjapojazdu.h"
#include "specyfikacjapocisku.h"
#include "specyfikacjaanimacji.h"
#include "graf.h"
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
		QList<SpecyfikacjaPojazdu*> specyfikacjePojazdow;
		QList<SpecyfikacjaPocisku*> specyfikacjePociskow;
		QList<SpecyfikacjaAnimacji*> specyfikacjeAnimacji;
		Tekstura* mapa;
		QPoint widok;
		QList<QPolygonF> przeszkody;
		Graf graf;
		PojazdGracza* pojazdGracza;
		Tekstura celownik;
		QList<PojazdObcy*> pojazdyObce;
		QList<Pocisk*> pociski;
		QList<Animacja*> animacje;
		QList<Animacja*> bonusy;
		bool zainicjalizowana;
		
		void odswiezWidok();
		void rysujMape(QPainter&);
		void rysujAnimacje(QPainter&);
		void rysujPociski(QPainter&);
		void rysujCelownik(QPainter&);
		
	public:
		Plansza(Ekran*);
		~Plansza();
		void dodajSpecyfikacje(SpecyfikacjaPojazdu*);
		void dodajSpecyfikacje(SpecyfikacjaPocisku*);
		void dodajSpecyfikacje(SpecyfikacjaAnimacji*);
		bool zaladuj(QString);
		void czysc();
		void rysuj();
	
	friend class Logika;
};

#endif // PLANSZA_H
