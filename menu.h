#ifndef MENU_H
#define MENU_H

#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"

class Menu{
	private:
		Ekran& ekran;
		BazaDanych& bazaDanych;
		Plansza& plansza;
		
		void zaladujSpecyfikecjeObiektow();
		
	public:
		Menu(Ekran&, BazaDanych&, Plansza&);
		void rysuj() const;
};

#endif // MENU_H
