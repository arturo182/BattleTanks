#ifndef MENU_H
#define MENU_H

#include <QPoint>
#include "silnik.h"

class Ekran;
class BazaDanych;
class Plansza;

class Menu{
	public:
		enum Tryb{
			TEST1,
			TEST2,
			TEST3
		};
		
	private:
		Ekran* ekran;
		BazaDanych* bazaDanych;
		Plansza* plansza;
		Tryb tryb;
		
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
		Silnik::Tryb odswiez(int, Akcja);
		void rysuj() const;
};

#endif // MENU_H
