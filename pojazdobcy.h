#ifndef POJAZDOBCY_H
#define POJAZDOBCY_H

#include "pojazd.h"

class PojazdObcy: public Pojazd{
	private:
		int v, w, vPoprzedni;
		bool ustawZwrot;
		bool szykujWystrzal;
		int licznik;
	
	public:
		PojazdObcy(const SpecyfikacjaPojazdu*, QPointF, int, float, int);
		int wystrzelPocisk();
	
	friend class Logika;
	// DEBUG
	friend class Plansza;
};

#endif // POJAZDOBCY_H
