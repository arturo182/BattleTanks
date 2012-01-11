#ifndef POJAZD_H
#define POJAZD_H

#include <QVector2D>
#include "obiekt.h"

#define MINIMALNA_ODLEGLOSC_CELOWNIKA 200

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
		
		Pojazd(SpecyfikacjaPojazdu*, QPointF, float);
		virtual ~Pojazd();
		QPointF punktWylotuLufy() const;
		void rysuj(QPainter&, QPoint) const;
};

#endif // POJAZD_H
