#ifndef PAUZA_H
#define PAUZA_H

#include "silnik.h"

#include <QPixmap>

class Pauza{
	public:
		enum Tryb{
			MENU_GLOWNE = 0,
			USTAWIENIA,
			KONIEC_ROZGRYWKI
		};

	private:
		Ekran *ekran;
		BazaDanych *bazaDanych;
		Plansza *plansza;
		QPixmap tlo;
		int pozycja;
		Tryb tryb;
		int glosnosc;
		QString rozdzielczosc;
		QString jakosc;
		QString sterowanie;

	public:
		Pauza(Ekran*, BazaDanych*, Plansza*);

		Silnik::Tryb odswiez(int, Silnik::Akcja);
		void rysuj() const;

		void ustawTlo();

	private:
		void wczytajUstawienia();
		void zapiszUstawienia();
};

#endif // PAUZA_H
