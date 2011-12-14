#ifndef POJAZD_H
#define POJAZD_H

#include "obiekt.h"
#include "specyfikacjapojazdu.h"

class Pojazd: public Obiekt{
	private:
		const SpecyfikacjaPojazdu& specyfikacja;
		int zdrowie;
		
	public:
		Pojazd(const SpecyfikacjaPojazdu&, QPointF);
		void rysuj(QPainter&) const;
};

#endif // POJAZD_H
