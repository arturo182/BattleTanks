#include <cmath>
#include <QTransform>
#include <QVector2D>
#include "pojazd.h"
#include "specyfikacjapojazdu.h"

Pojazd::Pojazd(SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusuKat):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	zwrotKorpusuKat(zwrotKorpusuKat),
	zwrotWiezyWzgledemKorpusuKat(0.0F),
	odlegloscCelu(200.0F),
	zdrowie(this->specyfikacja->wytrzymalosc){
	this->odswiezWektory();
}

Pojazd::~Pojazd(){}

void Pojazd::odswiezWektory(){
	this->zwrotKorpusuWektor.setX(cos(this->zwrotKorpusuKat));
	this->zwrotKorpusuWektor.setY(-sin(this->zwrotKorpusuKat));
	this->zwrotWiezyWektor.setX(cos(this->zwrotKorpusuKat + this->zwrotWiezyWzgledemKorpusuKat));
	this->zwrotWiezyWektor.setY(sin(-(this->zwrotKorpusuKat + this->zwrotWiezyWzgledemKorpusuKat)));
}

void Pojazd::rysuj(QPainter& painter, QPoint widok) const{
	QTransform transformacja;
	
	transformacja.rotateRadians(M_PI_2 - this->zwrotKorpusuKat);
	QPixmap teksturaKorpus = this->specyfikacja->teksturaKorpus.teksturaPrzeskalowana.transformed(transformacja);
	painter.drawPixmap(
		Obiekt::skala * this->pozycja - widok - QPoint(teksturaKorpus.width() / 2, teksturaKorpus.height() / 2),
		teksturaKorpus
	);
	
	transformacja.rotateRadians(-this->zwrotWiezyWzgledemKorpusuKat);
	QVector2D wektorPrzesuniecieOsiWiezy = this->zwrotWiezyWektor * this->specyfikacja->przesuniecieOsiDlaWiezy - this->zwrotKorpusuWektor * this->specyfikacja->przesuniecieOsiDlaKorpusu;
	QPixmap teksturaWieza = this->specyfikacja->teksturaWieza.teksturaPrzeskalowana.transformed(transformacja);
	painter.drawPixmap(
		Obiekt::skala * (this->pozycja + wektorPrzesuniecieOsiWiezy.toPointF()) - widok - QPoint(teksturaWieza.width() / 2, teksturaWieza.height() / 2),
		teksturaWieza
	);
}