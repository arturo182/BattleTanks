#ifndef MENU_H
#define MENU_H

#include "silnik.h"

class Ekran;
class BazaDanych;
class Plansza;

class Menu{
	private:
		Ekran* ekran;
		BazaDanych* bazaDanych;
		Plansza* plansza;
		
		void zaladujSpecyfikecjeObiektow();
		
	public:
		enum Akcja{
			GORA,
			DOL,
			PRAWO,
			LEWO,
			WYBIERZ,
			COFNIJ,
			BRAK
		};
		
		Menu(Ekran*, BazaDanych*, Plansza*);
		Silnik::Tryb odswiez(Akcja);
		void rysuj() const;
};

#endif // MENU_H
