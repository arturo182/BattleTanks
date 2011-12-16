#ifndef SPECYFIKACJAANIMACJI_H
#define SPECYFIKACJAANIMACJI_H

#include "tekstura.h" 

class SpecyfikacjaAnimacji{
	private:
		Tekstura tekstury;
		
	public:
		QSize iloscKlatek;
		
		SpecyfikacjaAnimacji(const QPixmap&, QSize);
	
	friend class Animacja;
};

#endif // SPECYFIKACJAANIMACJI_H
