#include <cmath>
#include "pocisk.h"
#include "specyfikacjapocisku.h"
#include <QDebug>

Pocisk::Pocisk(SpecyfikacjaPocisku* specyfikacja, bool pociskGracza, QPointF punktStartowy, QVector2D wektorKierunku, float dystansCalkowity):
	Obiekt(punktStartowy),
	specyfikacja(specyfikacja),
	pociskGracza(pociskGracza),
	punktStartowy(punktStartowy),
	wektorKierunku(wektorKierunku),
	dystansCalkowity(dystansCalkowity),
	czasLotuMilisekundy(0){
	QTransform transformacja;
	
	transformacja.rotateRadians(M_PI_2 + atan2(this->wektorKierunku.y(), this->wektorKierunku.x()));
	this->tekstura = new Tekstura(this->specyfikacja->tekstura.transformed(transformacja));
}

Pocisk::~Pocisk(){
	delete this->tekstura;
}

bool Pocisk::przemiesc(int milisekundy){
	float dystansAktualny;
	
	this->czasLotuMilisekundy += milisekundy;
	dystansAktualny = this->specyfikacja->predkosc * float(this->czasLotuMilisekundy) / 1000.0;
	if(dystansAktualny < this->dystansCalkowity){
		this->pozycja = this->punktStartowy + dystansAktualny * this->wektorKierunku.toPointF();
		return false;
	}else{
		this->pozycja = this->punktStartowy + this->dystansCalkowity * this->wektorKierunku.toPointF();
		return true;
	}
}

void Pocisk::rysuj(QPainter& painter, QPoint widok) const{
	painter.drawPixmap(
		Obiekt::skala * this->pozycja - widok - QPointF(this->tekstura->teksturaPrzeskalowana.width() / 2.0, this->tekstura->teksturaPrzeskalowana.height() / 2.0),
		this->tekstura->teksturaPrzeskalowana
	);
}
