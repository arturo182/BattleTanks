#ifndef POJAZDOBCY_H
#define POJAZDOBCY_H

#include "pojazd.h"

class PojazdObcy: public Pojazd{
	public:
		PojazdObcy(const SpecyfikacjaPojazdu*, QPointF, float, int);
		int wystrzelPocisk();
	
	friend class Logika;
};

#endif // POJAZDOBCY_H
