#ifndef POJAZDGRACZA_H
#define POJAZDGRACZA_H

#include "pojazd.h"

class PojazdGracza: public Pojazd{
	private:
		int* zasobyPociskow;
		int aktualnaBron;
	
	public:
		PojazdGracza(SpecyfikacjaPojazdu*, QPointF, float, int, int);
		~PojazdGracza();
	
	friend class Logika;
};

#endif // POJAZDGRACZA_H
