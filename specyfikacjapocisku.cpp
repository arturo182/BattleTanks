#include "specyfikacjapocisku.h"

SpecyfikacjaPocisku::SpecyfikacjaPocisku(const QPixmap& tekstura, int zasieg, int predkosc, int silaRazenie, int promienRazenie, const SpecyfikacjaAnimacji* specyfikacjaAnimacji):
	tekstura(tekstura),
	specyfikacjaAnimacji(specyfikacjaAnimacji),
	rozmiar(this->tekstura.teksturaOryginalna.size()),
	zasieg(zasieg),
	predkosc(predkosc),
	silaRazenie(silaRazenie),
	promienRazenie(promienRazenie){}
