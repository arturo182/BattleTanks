#ifndef POJAZD_H
#define POJAZD_H

#include <QVector2D>
#include "obiekt.h"

#define MINIMALNA_ODLEGLOSC_CELOWNIKA 50

class SpecyfikacjaPojazdu;

class Pojazd: public Obiekt{
	public:
		const SpecyfikacjaPojazdu* specyfikacja;
		float zwrotKorpusuKat;
		float zwrotWiezyWzgledemKorpusuKat;
		QVector2D zwrotKorpusuWektor;
		QVector2D zwrotWiezyWektor;
		QPolygonF otoczka;
		float celownikOdleglosc;
		int zdrowie;
		int aktualnaBron;

		Pojazd(const SpecyfikacjaPojazdu*, QPointF, float, int);
		virtual ~Pojazd();
		QPointF punktWylotuLufy() const;
		virtual int wystrzelPocisk() = 0;
		void rysuj(QPainter&, QPoint) const;
};

#endif // POJAZD_H
