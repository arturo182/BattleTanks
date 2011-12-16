#ifndef ANIMACJA_H
#define ANIMACJA_H

#include "obiekt.h"

class SpecyfikacjaAnimacji;

class Animacja: public Obiekt{
	public:
		const SpecyfikacjaAnimacji* specyfikacja;
		
		Animacja(SpecyfikacjaAnimacji*, QPointF);
		void rysuj(QPainter&, QPoint) const;
};

#endif // ANIMACJA_H
