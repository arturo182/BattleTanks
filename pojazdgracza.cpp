#include "pojazdgracza.h"

PojazdGracza::PojazdGracza(const SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusu, int iloscRodzajowPociskow):
	Pojazd(specyfikacja, pozycja, zwrotKorpusu, 0),
	iloscRodzajowPociskow(iloscRodzajowPociskow),
	zasobyPociskow(new int[this->iloscRodzajowPociskow]){
	for(int i = 0; i < this->iloscRodzajowPociskow; i++)
		this->zasobyPociskow[i] = 0;
}

PojazdGracza::~PojazdGracza(){
	delete[] this->zasobyPociskow;
}

bool PojazdGracza::dodajPociski(int numerPociskow, int iloscPociskow){
	if(numerPociskow >= this->iloscRodzajowPociskow || this->zasobyPociskow[numerPociskow] < 0)
		return false;
	this->zasobyPociskow[numerPociskow] += iloscPociskow;
	return true;
}

int PojazdGracza::wystrzelPocisk(){
	if(this->zasobyPociskow[this->aktualnaBron] == 0)
		return -1;
	
	int wystrzelonyPocisk = this->aktualnaBron;
	if(this->zasobyPociskow[this->aktualnaBron] > 0)
		if(--this->zasobyPociskow[this->aktualnaBron] == 0)
			this->ustawBron();
	
	return wystrzelonyPocisk;
}

bool PojazdGracza::ustawBron(){
	return this->zasobyPociskow[this->aktualnaBron] != 0 || this->zmienBron(1);
}

bool PojazdGracza::zmienBron(int kierunek){
	int bron = this->aktualnaBron;
	int zmiana = kierunek < 0 ? -1 : 1;
	
	do{
		bron += zmiana;
		if(bron == this->iloscRodzajowPociskow)
			bron = 0;
		else if(bron < 0)
			bron = this->iloscRodzajowPociskow - 1;
	}while(this->zasobyPociskow[bron] == 0 && bron != this->aktualnaBron);
	
	if(bron == this->aktualnaBron)
		return false;
	
	this->aktualnaBron = bron;
	return true;
}
