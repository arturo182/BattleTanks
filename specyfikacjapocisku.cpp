#include "specyfikacjapocisku.h"

SpecyfikacjaPocisku::SpecyfikacjaPocisku(const QPixmap& tekstura, int zasieg, int predkosc, int silaRazenie, int promienRazenie):
	tekstura(tekstura),
	rozmiar(this->tekstura.size()),
	zasieg(zasieg),
	predkosc(predkosc),
	silaRazenie(silaRazenie),
	promienRazenie(promienRazenie){}
