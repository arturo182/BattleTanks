#include "animacja.h"
#include "specyfikacjaanimacji.h"

Animacja::Animacja(SpecyfikacjaAnimacji* specyfikacja, QPointF pozycja):
	Obiekt(pozycja),
	specyfikacja(specyfikacja){}

void Animacja::rysuj(QPainter& painter, QPoint widok) const{
}
