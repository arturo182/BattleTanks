#include "pojazdobcy.h"

PojazdObcy::PojazdObcy(SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusu, SpecyfikacjaPocisku* pociski):
	Pojazd(specyfikacja, pozycja, zwrotKorpusu),
	pociski(pociski){}
