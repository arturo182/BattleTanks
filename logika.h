#ifndef LOGIKA_H
#define LOGIKA_H

#include "plansza.h"

class Logika{
	private:
		Plansza& plansza;
		
	public:
		Logika(Plansza&);
		void odswiez(double, double, int, int, int, bool);
		
	friend class Pojazd;
	friend class Pocisk;
	friend class Animacja;
};

#endif // LOGIKA_H
