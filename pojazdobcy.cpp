#include "pojazdobcy.h"

PojazdObcy::PojazdObcy(const SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusu, int bron):
	Pojazd(specyfikacja, pozycja, zwrotKorpusu, bron){}
	
int PojazdObcy::wystrzelPocisk(){
	return this->aktualnaBron;
}
