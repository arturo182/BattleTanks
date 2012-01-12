#ifndef POJAZDGRACZA_H
#define POJAZDGRACZA_H

#include "pojazd.h"

class PojazdGracza: public Pojazd{
	private:
		int iloscRodzajowPociskow;
		int* zasobyPociskow;
	
	public:
		PojazdGracza(const SpecyfikacjaPojazdu*, QPointF, float, int);
		~PojazdGracza();
		bool dodajPociski(int, int);
		int wystrzelPocisk();
		inline int zapasPociskow() const;
		bool ustawBron();
		bool zmienBron(int);
};

int PojazdGracza::zapasPociskow() const{
	return this->zasobyPociskow[this->aktualnaBron];
}

#endif // POJAZDGRACZA_H
