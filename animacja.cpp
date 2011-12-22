#include "animacja.h"
#include "specyfikacjaanimacji.h"

Animacja::Animacja(SpecyfikacjaAnimacji* specyfikacja, QPointF pozycja):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	status(true),
	czasMilisekundy(0),
	aktualnaKlatka(0){}

void Animacja::odswiez(int milisekundy){
	if(!this->status)
		return;
	
	this->czasMilisekundy += milisekundy;
	if(this->specyfikacja->powtarzaj || this->czasMilisekundy < this->specyfikacja->czasTrwaniaMilisekundy)
		this->aktualnaKlatka = (this->specyfikacja->iloscKlatek.width() * this->specyfikacja->iloscKlatek.height() * this->czasMilisekundy / this->specyfikacja->czasTrwaniaMilisekundy) % (this->specyfikacja->iloscKlatek.width() * this->specyfikacja->iloscKlatek.height());
	else
		this->status = false;
}

void Animacja::rysuj(QPainter& painter, QPoint widok) const{
	if(!this->status)
		return;
	
	painter.drawPixmap(
		Obiekt::skala * this->pozycja - widok - QPoint(this->specyfikacja->tekstury.teksturaPrzeskalowana.width() / (2 * this->specyfikacja->iloscKlatek.width()), this->specyfikacja->tekstury.teksturaPrzeskalowana.height() / (2 * this->specyfikacja->iloscKlatek.height())),
		this->specyfikacja->tekstury.teksturaPrzeskalowana,
		QRect(
			(this->aktualnaKlatka % this->specyfikacja->iloscKlatek.width()) * this->specyfikacja->tekstury.teksturaPrzeskalowana.width() / this->specyfikacja->iloscKlatek.width(),
			(this->aktualnaKlatka / this->specyfikacja->iloscKlatek.width()) * this->specyfikacja->tekstury.teksturaPrzeskalowana.height() / this->specyfikacja->iloscKlatek.height(),
			this->specyfikacja->tekstury.teksturaPrzeskalowana.width() / this->specyfikacja->iloscKlatek.width(),
			this->specyfikacja->tekstury.teksturaPrzeskalowana.height() / this->specyfikacja->iloscKlatek.height()
		)
	);
}
