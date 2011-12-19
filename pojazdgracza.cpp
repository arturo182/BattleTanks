#include "pojazdgracza.h"

PojazdGracza::PojazdGracza(SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusu, int iloscPociskow, int domyslnaBron):
	Pojazd(specyfikacja, pozycja, zwrotKorpusu),
	aktualnaBron(domyslnaBron){
	this->zasobyPociskow = new int[iloscPociskow];
	for(int i = 0; i < iloscPociskow; i++)
		this->zasobyPociskow[i] = 0;
}

PojazdGracza::~PojazdGracza(){
	delete[] this->zasobyPociskow;
}
