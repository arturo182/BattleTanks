#include <cmath>
#include "pocisk.h"
#include "specyfikacjapocisku.h"
#include <QDebug>

Pocisk::Pocisk(SpecyfikacjaPocisku* specyfikacja, bool pociskGracza, QPointF punktStartowy, QVector2D wektorKierunku, float dystansCalkowity):
	Obiekt(punktStartowy),
	specyfikacja(specyfikacja),
	status(true),
	pociskGracza(pociskGracza),
	punktStartowy(punktStartowy),
	wektorKierunku(wektorKierunku),
	dystansCalkowity(dystansCalkowity),
	czasMilisekundy(0){
	QTransform transformacja;
	
	transformacja.rotateRadians(M_PI_2 + atan2(this->wektorKierunku.y(), this->wektorKierunku.x()));
	this->tekstura = new Tekstura(this->specyfikacja->tekstura.transformed(transformacja));
}

Pocisk::~Pocisk(){
	delete this->tekstura;
}

void Pocisk::odswiez(int milisekundy){
	if(!this->status)
		return;
		
	this->czasMilisekundy += milisekundy;
	float dystansAktualny = this->specyfikacja->predkosc * float(this->czasMilisekundy) / 1000.0;
	if(dystansAktualny < this->dystansCalkowity)
		this->pozycja = this->punktStartowy + dystansAktualny * this->wektorKierunku.toPointF();
	else{
		this->pozycja = this->punktStartowy + this->dystansCalkowity * this->wektorKierunku.toPointF();
		this->status = false;
	}
}

void Pocisk::rysuj(QPainter& painter, QPoint widok) const{
	if(!this->status)
		return;
		
	painter.drawPixmap(
		Obiekt::skala * this->pozycja - widok - QPointF(this->tekstura->teksturaPrzeskalowana.width() / 2.0, this->tekstura->teksturaPrzeskalowana.height() / 2.0),
		this->tekstura->teksturaPrzeskalowana
	);
}
