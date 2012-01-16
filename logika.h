#ifndef LOGIKA_H
#define LOGIKA_H

#include <cmath>
#include <QPointF>
#include <QSize>
#include <QPolygonF>
#include <QVector2D>
#include "plansza.h"
#include "pojazdobcy.h"

#define PREDKOSC_CELOWNIKA 300
#define PROMIEN_AKTYWNOSCI_OBCYCH_POJAZDOW 2000
#define WSPOLCZYNNIK_PREDKOSCI_OBCYCH_POJAZDOW 0.8
#define CZAS_OCZEKIWANIA_OBCYCH_POJAZDOW 2000
#define ODLEGLOSC_OD_PUNKTU_WYJSCIA 50

class Logika{
	private:
		enum RodzajKolizji{
			KRAWEDZIE_MAPY = 1,
			PRZESZKODY = 2,
			POJAZDY = 4,
			POJAZDY2 = 8
		};
		
		Plansza* plansza;
		
		inline float odleglosc(QPointF, QPointF) const;
		inline float wyznacznikMacierzyWspolliniowosci(QPointF, QPointF, QPointF) const;
		bool sprawdzOtoczki(QPolygonF&, QPolygonF&) const;
		QPolygonF wyznaczOtoczke(const Pojazd&) const;
		void inicjalizuj() const;
		bool sprawdzKolizje(Pojazd&, int) const;
		bool przemiescKorpus(Pojazd&, float, float, float);
		void obrocWieze(Pojazd&, int, float);
		void zmienZasieg(Pojazd&, int, float);
		bool wystrzelPocisk(Pojazd&, bool);
		void odswiezPojazdGracza(float, float, int, int, int, bool, float);
		inline bool pojazdNaWierzcholku(const PojazdObcy&) const;
		void odswiezObcePojazdy(int, float);
		
	public:
		Logika(Plansza*);
		void odswiez(int, float, float, int, int, int, bool);
};

float Logika::odleglosc(QPointF a, QPointF b) const{
	return sqrt(pow(a.x() - b.x(), 2.0) + pow(a.y() - b.y(), 2.0));
}

float Logika::wyznacznikMacierzyWspolliniowosci(QPointF a, QPointF b, QPointF c) const{
	return a.x() * b.y() + b.x() * c.y() + c.x() * a.y() - b.y() * c.x() - c.y() * a.x() - a.y() * b.x();
}

bool Logika::pojazdNaWierzcholku(const PojazdObcy& pojazd) const{
	return pojazd.pozycja == this->plansza->graf.pozycjaWierzcholka(pojazd.v);
}

#endif // LOGIKA_H
