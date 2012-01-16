#include "specyfikacjapocisku.h"

SpecyfikacjaPocisku::SpecyfikacjaPocisku(const QPixmap& tekstura, const QString& dzwiekWystrzalu, int zasieg, int predkosc, int silaRazenia, int promienRazenia, const SpecyfikacjaAnimacji* specyfikacjaAnimacji):
	tekstura(tekstura),
	specyfikacjaAnimacji(specyfikacjaAnimacji),
	dzwiekWystrzalu(dzwiekWystrzalu),
	rozmiar(this->tekstura.teksturaOryginalna.size()),
	zasieg(zasieg),
	predkosc(predkosc),
	silaRazenia(silaRazenia),
	promienRazenia(promienRazenia){}
