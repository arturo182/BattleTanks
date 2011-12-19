#ifndef POCISK_H
#define POCISK_H

#include <QVector2D>
#include "obiekt.h"
#include "tekstura.h"

class SpecyfikacjaPocisku;

class Pocisk: public Obiekt{
	private:
		const SpecyfikacjaPocisku* specyfikacja;
		Tekstura* tekstura;
		bool pociskGracza;
		QPointF punktStartowy;
		QVector2D wektorKierunku;
		float dystansCalkowity;
		int czasLotuMilisekundy;
	
	public:
		Pocisk(SpecyfikacjaPocisku*, bool, QPointF, QVector2D, float);
		~Pocisk();
		bool przemiesc(int);
		void rysuj(QPainter&, QPoint) const;
	
	friend class Logika;
};

#endif // POCISK_H
