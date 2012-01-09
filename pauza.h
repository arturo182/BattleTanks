#ifndef PAUZA_H
#define PAUZA_H

#include "silnik.h"

class Pauza{
	private:
		Ekran *ekran;
		Plansza *plansza;
		int pozycja;

	public:
		Pauza(Ekran*, Plansza*);

		Silnik::Tryb odswiez(int, Silnik::Akcja);
		void rysuj() const;
};

#endif // PAUZA_H
