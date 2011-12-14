#ifndef POCISK_H
#define POCISK_H

#include "obiekt.h"

class Pocisk: public Obiekt{
	public:
		Pocisk(QPointF);
		void rysuj(QPainter&) const;
};

#endif // POCISK_H
