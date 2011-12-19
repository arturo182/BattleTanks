#ifndef SPECYFIKACJAPOJAZDU_H
#define SPECYFIKACJAPOJAZDU_H

#include "tekstura.h"

class SpecyfikacjaPojazdu{
	private:
		Tekstura teksturaKorpus;
		Tekstura teksturaWieza;
		
	public:
		QSize rozmiarKorpus;
		QSize rozmiarWieza;
		int przesuniecieOsiDlaKorpusu;
		int przesuniecieOsiDlaWiezy;
		int predkoscMaksymalnaPojazdu;
		float predkoscRotacjiWiezy;
		int wytrzymalosc;
		
		SpecyfikacjaPojazdu(const QPixmap&, const QPixmap&, int, int, int, float, int);
	
	friend class Pojazd;
};

#endif // SPECYFIKACJAPOJAZDU_H
