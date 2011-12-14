#ifndef ANIMACJA_H
#define ANIMACJA_H

#include "obiekt.h"

class SpecyfikacjaAnimacji;

class Animacja: public Obiekt{
	private:
		const SpecyfikacjaAnimacji* specyfikacja;
		
	public:
		Animacja(SpecyfikacjaAnimacji*, QPointF);
		void rysuj(QPainter&) const;
};

#endif // ANIMACJA_H
