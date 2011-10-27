#include "tekstury.h"

Tekstury::Tekstury(QString sciezka, QSize ilosc_klatek):
	QPixmap(sciezka),
	ilosc_klatek(ilosc_klatek),
	rozmiar_klatki(this->size().width() / this->ilosc_klatek.width(), this->size().height() / this->ilosc_klatek.height()){}
