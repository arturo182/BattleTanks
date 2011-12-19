#ifndef POJAZD_H
#define POJAZD_H

#include <QVector2D>
#include "obiekt.h"

class SpecyfikacjaPojazdu;

class Pojazd: public Obiekt{
	public:
		const SpecyfikacjaPojazdu* specyfikacja;
		float zwrotKorpusuKat;
		float zwrotWiezyWzgledemKorpusuKat;
		QVector2D zwrotKorpusuWektor;
		QVector2D zwrotWiezyWektor;
		float odlegloscCelu;
		int zdrowie;
		
		Pojazd(SpecyfikacjaPojazdu*, QPointF, float);
		virtual ~Pojazd();
		void odswiezWektory();
		void rysuj(QPainter&, QPoint) const;
};

#endif // POJAZD_H
