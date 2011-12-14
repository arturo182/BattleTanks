#include "pojazd.h"
#include "specyfikacjapojazdu.h"

Pojazd::Pojazd(SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	zdrowie(this->specyfikacja->wytrzymalosc){}

void Pojazd::rysuj(QPainter& painter) const{
}
