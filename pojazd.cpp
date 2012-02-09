#include <cmath>
#include <QTransform>
#include <QVector2D>
#include "pojazd.h"
#include "specyfikacjapojazdu.h"

Pojazd::Pojazd(const SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusuKat, int aktualnaBron):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	zwrotKorpusuKat(zwrotKorpusuKat),
	zwrotWiezyWzgledemKorpusuKat(0.0F),
	zwrotKorpusuWektor(cos(this->zwrotKorpusuKat), -sin(this->zwrotKorpusuKat)),
	zwrotWiezyWektor(cos(this->zwrotKorpusuKat + this->zwrotWiezyWzgledemKorpusuKat), -sin(this->zwrotKorpusuKat + this->zwrotWiezyWzgledemKorpusuKat)),
	celownikOdleglosc(MINIMALNA_ODLEGLOSC_CELOWNIKA),
	zdrowie(this->specyfikacja->wytrzymalosc),
	aktualnaBron(aktualnaBron){}

Pojazd::~Pojazd(){}

QPointF Pojazd::punktWylotuLufy() const{
	return this->pozycja + (((this->specyfikacja->przesuniecieOsiDlaWiezy + this->specyfikacja->rozmiarWieza.height() / 2.0) * this->zwrotWiezyWektor) - (this->specyfikacja->przesuniecieOsiDlaKorpusu * this->zwrotKorpusuWektor)).toPointF();
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

	painter.fillRect(QRectF(Obiekt::skala * QPointF(this->pozycja.x() - 22, this->pozycja.y() - 30) - widok, Obiekt::skala * QSize(40, 4)), Qt::red);
	painter.fillRect(QRectF(Obiekt::skala * QPointF(this->pozycja.x() - 22, this->pozycja.y() - 30) - widok, Obiekt::skala * QSize(40 * this->zdrowie / this->specyfikacja->wytrzymalosc, 4)), Qt::green);
}
