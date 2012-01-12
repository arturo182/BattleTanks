#include <cmath>
#include "pocisk.h"
#include "specyfikacjapocisku.h"
#include <QDebug>

Pocisk::Pocisk(const SpecyfikacjaPocisku* specyfikacja, bool pociskGracza, QPointF punktStartowy, QVector2D wektorKierunku, float dystansCalkowity):
	Obiekt(punktStartowy),
	specyfikacja(specyfikacja),
	status(true),
	pociskGracza(pociskGracza),
	punktStartowy(punktStartowy),
	wektorKierunku(wektorKierunku),
	dystansCalkowity(dystansCalkowity),
	dystansAktualny(0.0),
	czasMilisekundy(0){
	QTransform transformacja;
	
	transformacja.rotateRadians(M_PI_2 + atan2(this->wektorKierunku.y(), this->wektorKierunku.x()));
	this->tekstura = new Tekstura(this->specyfikacja->tekstura.teksturaOryginalna.transformed(transformacja));
}

Pocisk::~Pocisk(){
	delete this->tekstura;
}

void Pocisk::odswiez(int milisekundy){
	if(!this->status)
		return;
		
	this->czasMilisekundy += milisekundy;
	this->dystansAktualny = this->specyfikacja->predkosc * float(this->czasMilisekundy) / 1000.0;
	
	if(this->dystansAktualny > this->dystansCalkowity){
		this->dystansAktualny = this->dystansCalkowity;
		this->status = false;
	}
	
	this->pozycja = this->punktStartowy + this->dystansAktualny * this->wektorKierunku.toPointF();
}

void Pocisk::rysuj(QPainter& painter, QPoint widok) const{
	if(!this->status || this->dystansAktualny < this->specyfikacja->rozmiar.height())
		return;
	
	painter.drawPixmap(
		Obiekt::skala * (this->punktStartowy + (this->dystansAktualny - this->specyfikacja->rozmiar.height() / 2.0) * this->wektorKierunku.toPointF()) - widok - QPointF(this->tekstura->teksturaPrzeskalowana.width() / 2.0, this->tekstura->teksturaPrzeskalowana.height() / 2.0),
		this->tekstura->teksturaPrzeskalowana
	);
}
