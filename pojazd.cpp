#include "pojazd.h"

Pojazd::Pojazd(const SpecyfikacjaPojazdu& specyfikacja, QPointF pozycja):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	zdrowie(this->specyfikacja.wytrzymalosc){}

void Pojazd::rysuj(QPainter& painter) const{
}
