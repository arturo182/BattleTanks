#ifndef POJAZDOBCY_H
#define POJAZDOBCY_H

#include "pojazd.h"

class SpecyfikacjaPocisku;

class PojazdObcy: public Pojazd{
	private:
		SpecyfikacjaPocisku* pociski;
	
	public:
		PojazdObcy(SpecyfikacjaPojazdu*, QPointF, float, SpecyfikacjaPocisku*);
	
	friend class Logika;
};

#endif // POJAZDOBCY_H
