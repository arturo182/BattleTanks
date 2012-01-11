#ifndef LOGIKA_H
#define LOGIKA_H

#include <QPointF>
#include <QSize>
#include <QPolygonF>
#include <QVector2D>

#define PREDKOSC_CELOWNIKA 300

class Plansza;
class Pojazd;

class Logika{
	private:
		enum RodzajKolizji{
			KRAWEDZIE_MAPY = 1,
			PRZESZKODY = 2,
			POJAZDY = 3
		};
	
		Plansza* plansza;
		
		inline float wyznacznikMacierzyWspolliniowosci(QPointF, QPointF, QPointF) const;
		bool sprawdzOtoczki(QPolygonF&, QPolygonF&) const;
		QPolygonF wyznaczOtoczke(QPointF, QSize, QVector2D) const;
		bool sprawdzKolizje(Pojazd&, int) const;
		bool przemiescKorpus(Pojazd&, float, float, float, int);
		void obrocWieze(Pojazd&, float, float);
		
	public:
		Logika(Plansza*);
		void odswiez(int, float, float, int, int, int, bool);
};

float Logika::wyznacznikMacierzyWspolliniowosci(QPointF a, QPointF b, QPointF c) const{
	return a.x() * b.y() + b.x() * c.y() + c.x() * a.y() - b.y() * c.x() - c.y() * a.x() - a.y() * b.x();
}

#endif // LOGIKA_H
