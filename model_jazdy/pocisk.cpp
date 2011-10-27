#include <cmath>
#include "pocisk.h"

Pocisk::Pocisk(QPointF pozycja_poczatkowa, QPointF pozycja_docelowa, double predkosc, QTime czas_wystrzalu):
	pozycja_poczatkowa(pozycja_poczatkowa),
	pozycja_docelowa(pozycja_docelowa),
	pozycja_aktualna(pozycja_poczatkowa),
	predkosc(predkosc),
	czas_wystrzalu(czas_wystrzalu),
	dystans_calkowity(
		sqrt(
			pow(pozycja_docelowa.x() - pozycja_poczatkowa.x(), 2)
			+ pow(pozycja_docelowa.y() - pozycja_poczatkowa.y(), 2)
		)
	){}

bool Pocisk::przesun(QTime czas_aktualny){
	double dystans_aktualny;
	
	dystans_aktualny = this->predkosc * this->czas_wystrzalu.msecsTo(czas_aktualny) / 1000;
	if(dystans_aktualny < this->dystans_calkowity){
		this->pozycja_aktualna = this->pozycja_poczatkowa + QPointF(
			(this->pozycja_docelowa.x() - this->pozycja_poczatkowa.x()) * dystans_aktualny / this->dystans_calkowity,
			(this->pozycja_docelowa.y() - this->pozycja_poczatkowa.y()) * dystans_aktualny / this->dystans_calkowity
		);
		return false;
	}else{
		this->pozycja_aktualna = this->pozycja_docelowa;
		return true;
	}
}
