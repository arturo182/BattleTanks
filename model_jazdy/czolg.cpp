#include "czolg.h"

Czolg::Czolg(QPointF pozycja, QPixmap korpus, QPixmap wieza, int przesuniecie_osi_wiezy_dla_korpusu, int przesuniecie_osi_wiezy_dla_wiezy, double kierunek, double predkosc_maksymalna, double predkosc_obrotu_wieza):
	pozycja(pozycja),
	rozmiar(korpus.size()),
	korpus(korpus),
	wieza(wieza),
	przesuniecie_osi_wiezy_dla_korpusu(przesuniecie_osi_wiezy_dla_korpusu),
	przesuniecie_osi_wiezy_dla_wiezy(przesuniecie_osi_wiezy_dla_wiezy),
	kierunek(kierunek),
	kierunek_wiezy(0.0),
	predkosc_maksymalna(predkosc_maksymalna),
	predkosc_obrotu_wieza(predkosc_obrotu_wieza){}
