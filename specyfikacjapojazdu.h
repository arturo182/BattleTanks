#ifndef SPECYFIKACJAPOJAZDU_H
#define SPECYFIKACJAPOJAZDU_H

#include "tekstura.h"

class SpecyfikacjaPojazdu{
	private:
		Tekstura teksturaKorpus;
		Tekstura teksturaWieza;
		int przesuniecieWiezy;
		int predkoscMaksymalna;
		int wytrzymalosc;
		
	public:
		SpecyfikacjaPojazdu(const QPixmap&, const QPixmap&, int, int, int);
	
	friend class Pojazd;
};

#endif // SPECYFIKACJAPOJAZDU_H
