#ifndef SPECYFIKACJAANIMACJI_H
#define SPECYFIKACJAANIMACJI_H

#include "tekstura.h"

class SpecyfikacjaAnimacji{
	private:
		Tekstura tekstury;
		
	public:
		QSize iloscKlatek;
		int czasTrwaniaMilisekundy;
		bool powtarzaj;
		
		SpecyfikacjaAnimacji(const QPixmap&, QSize, int, bool = false);
	
	friend class Animacja;
};

#endif // SPECYFIKACJAANIMACJI_H
