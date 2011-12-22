#ifndef LOGIKA_H
#define LOGIKA_H

#include <QPointF>
#include <QSize>

class Plansza;
class Pojazd;

class Logika{
	private:
		Plansza* plansza;
		
		bool sprawdzKolizje(QSize, QPointF, float) const;
		bool przemiesc(Pojazd&, float, float, float);
		
	public:
		Logika(Plansza*);
		void odswiez(int, float, float, int, int, int, bool);
};

#endif // LOGIKA_H
