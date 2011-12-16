#ifndef POJAZD_H
#define POJAZD_H

#include "obiekt.h"

class SpecyfikacjaPojazdu;

class Pojazd: public Obiekt{
	public:
		const SpecyfikacjaPojazdu* specyfikacja;
		float zwrotKorpusu;
		float zwrotWiezy;
		float odlegloscCelu;
		int zdrowie;
		
		Pojazd(SpecyfikacjaPojazdu*, QPointF, float);
		void rysuj(QPainter&, QPoint) const;
};

#endif // POJAZD_H
