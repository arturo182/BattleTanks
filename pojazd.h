#ifndef POJAZD_H
#define POJAZD_H

#include "obiekt.h"

class SpecyfikacjaPojazdu;

class Pojazd: public Obiekt{
	private:
		const SpecyfikacjaPojazdu* specyfikacja;
		int zdrowie;
		
	public:
		Pojazd(SpecyfikacjaPojazdu*, QPointF);
		void rysuj(QPainter&) const;
};

#endif // POJAZD_H
