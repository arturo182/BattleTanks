#ifndef ANIMACJA_H
#define ANIMACJA_H

#include "obiekt.h"
#include "specyfikacjaanimacji.h"

class Animacja: public Obiekt{
	private:
		const SpecyfikacjaAnimacji& specyfikacja;
		
	public:
		Animacja(const SpecyfikacjaAnimacji&, QPointF);
		void rysuj(QPainter&) const;
};

#endif // ANIMACJA_H
