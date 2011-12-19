#include "specyfikacjapocisku.h"

SpecyfikacjaPocisku::SpecyfikacjaPocisku(const QPixmap& tekstura, int dystansMaksymalny, int predkosc, int silaRazenie, int promienRazenie):
	tekstura(tekstura),
	rozmiar(this->tekstura.size()),
	dystansMaksymalny(dystansMaksymalny),
	predkosc(predkosc),
	silaRazenie(silaRazenie),
	promienRazenie(promienRazenie){}
