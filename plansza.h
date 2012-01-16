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
	public:
		enum TrybGry{
			DEMOLKA,
			LABIRYNT
		};
		
	private:
		enum StatusGry{
			PLANSZA_PUSTA,
			NIEZAINICJALIZOWANA,
			ROZGRYWKA_TRWA,
			ROZGRYWKA_ZAKONCZONA
		} status;
		
		Ekran* ekran;
		QList<SpecyfikacjaPojazdu*> specyfikacjePojazdow;
		QList<SpecyfikacjaPocisku*> specyfikacjePociskow;
		QList<SpecyfikacjaAnimacji*> specyfikacjeAnimacji;
		TrybGry tryb;
		int idPlanszy;
		Tekstura* mapa;
		QPoint wyjscie;
		QPoint widok;
		QList<QPolygonF> przeszkody;
		Graf graf;
		PojazdGracza* pojazdGracza;
		Tekstura celownik;
		QList<PojazdObcy*> pojazdyObce;
		QList<Pocisk*> pociski;
		QList<Animacja*> animacje;
		QList<Animacja*> bonusy;
		
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
		bool zaladuj(int, QString);
		void czysc();
		void rysuj();
		inline int id() const;
		inline bool koniecGry() const;
	
	friend class Logika;
};

int Plansza::id() const{
	return this->idPlanszy;
}

bool Plansza::koniecGry() const{
	return this->status == 3;
}

#endif // PLANSZA_H
