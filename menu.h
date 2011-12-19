#ifndef MENU_H
#define MENU_H

#include <QPoint>
#include "silnik.h"

class QTreeWidget;

class BazaDanych;
class Plansza;
class Ekran;

class Menu{
  public:
	enum Tryb{
	  WYBOR_PROFILU,
	  MENU_GLOWNE,
	  WYBOR_TRYBU,
	  WYBOR_MAPY,
	  REKORDY,
	  USTAWIENIA
	};

  private:
	QTreeWidget *drzewko;
	Ekran* ekran;
	BazaDanych* bazaDanych;
	Plansza* plansza;
	Tryb tryb;
	int pozycja;
	int zaznaczenie;
	int idGracza;

  public:
	enum Akcja{
	  GORA,
	  DOL,
	  PRAWO,
	  LEWO,
	  WYBIERZ,
	  WYBIERZ2,
	  COFNIJ,
	  BRAK
	};

	Menu(Ekran*, BazaDanych*, Plansza*);
	Silnik::Tryb odswiez(int, Akcja);
	void rysuj() const;
};

#endif // MENU_H
