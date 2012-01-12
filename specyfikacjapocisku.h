#ifndef SPECYFIKACJAPOCISKU_H
#define SPECYFIKACJAPOCISKU_H

#include "tekstura.h"

class SpecyfikacjaAnimacji;

class SpecyfikacjaPocisku{
	private:
		Tekstura tekstura;
		const SpecyfikacjaAnimacji* specyfikacjaAnimacji;
		
	public:
		QSize rozmiar;
		int zasieg;
		int predkosc;
		int silaRazenie;
		int promienRazenie;
		
		SpecyfikacjaPocisku(const QPixmap&, int, int, int, int, const SpecyfikacjaAnimacji*);
		inline const SpecyfikacjaAnimacji* animacja() const;
	
	friend class Pocisk;
	friend class Plansza;
};

const SpecyfikacjaAnimacji* SpecyfikacjaPocisku::animacja() const{
	return this->specyfikacjaAnimacji;
}

#endif // SPECYFIKACJAPOCISKU_H
