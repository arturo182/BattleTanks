#include "specyfikacjaanimacji.h"

SpecyfikacjaAnimacji::SpecyfikacjaAnimacji(const QPixmap& tekstury, QSize iloscKlatek, int czasTrwaniaMilisekundy, bool powtarzaj):
	tekstury(tekstury),
	iloscKlatek(iloscKlatek),
	czasTrwaniaMilisekundy(czasTrwaniaMilisekundy),
	powtarzaj(powtarzaj){}
