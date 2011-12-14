#ifndef SPECYFIKACJAANIMACJI_H
#define SPECYFIKACJAANIMACJI_H

#include "tekstura.h" 

class SpecyfikacjaAnimacji{
	private:
		Tekstura tekstury;
		QSize iloscKlatek;
		
	public:
		SpecyfikacjaAnimacji(const QPixmap&, QSize);
	
	friend class Animacja;
};

#endif // SPECYFIKACJAANIMACJI_H
