#ifndef SPECYFIKACJAPOCISKU_H
#define SPECYFIKACJAPOCISKU_H

#include "tekstura.h" 

class SpecyfikacjaPocisku{
	private:
		QPixmap tekstura;
		
	public:
		QSize rozmiar;
		int zasieg;
		int predkosc;
		int silaRazenie;
		int promienRazenie;
		
		SpecyfikacjaPocisku(const QPixmap&, int, int, int, int);
	
	friend class Pocisk;
};

#endif // SPECYFIKACJAPOCISKU_H
