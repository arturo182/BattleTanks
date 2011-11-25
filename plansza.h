#ifndef PLANSZA_H
#define PLANSZA_H

#include <QPixmap>
#include <QList>
#include "ekran.h"
#include "pojazd.h"
#include "pocisk.h"
#include "animacja.h"

class Plansza{
	private:
		Ekran& ekran;
		QPixmap mapa;
		QList<Obiekt*> obiekty;
		Pojazd* pojazdGracza;
		QList<Pojazd*> pojazdyObce;
		QList<Pocisk*> pociski;
		QList<Animacja*> animacje;
		QList<Animacja*> bonusy;
		
	public:
		Plansza(Ekran&);
		bool zaladuj(const char*);
		void czysc();
		void rysuj() const;
};

#endif // PLANSZA_H
