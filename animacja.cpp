#include "animacja.h"

Animacja::Animacja(const SpecyfikacjaAnimacji& specyfikacja, QPointF pozycja):
	Obiekt(pozycja),
	specyfikacja(specyfikacja){}

void Animacja::rysuj(QPainter& painter) const{
}
