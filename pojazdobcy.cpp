#include "pojazdobcy.h"

PojazdObcy::PojazdObcy(const SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, int wierzcholek, float zwrotKorpusu, int bron):
	Pojazd(specyfikacja, pozycja, zwrotKorpusu, bron),
	v(wierzcholek),
	w(-1),
	ustawZwrot(false){}
	
int PojazdObcy::wystrzelPocisk(){
	return this->aktualnaBron;
}
