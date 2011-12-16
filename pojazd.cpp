#include <cmath>
#include <QTransform>
#include <QVector2D>
#include "pojazd.h"
#include "specyfikacjapojazdu.h"

Pojazd::Pojazd(SpecyfikacjaPojazdu* specyfikacja, QPointF pozycja, float zwrotKorpusu):
	Obiekt(pozycja),
	specyfikacja(specyfikacja),
	zwrotKorpusu(zwrotKorpusu),
	zwrotWiezy(0.0F),
	odlegloscCelu(200.0F),
	zdrowie(this->specyfikacja->wytrzymalosc){}

void Pojazd::rysuj(QPainter& painter, QPoint widok) const{
	QTransform transformacja;
	transformacja.rotateRadians(M_PI_2 - this->zwrotKorpusu);
	
	QPixmap teksturaKorpus = this->specyfikacja->teksturaKorpus.teksturaPrzeskalowana.transformed(transformacja);
	painter.drawPixmap(
		Obiekt::skala * this->pozycja - QPoint(teksturaKorpus.width() / 2, teksturaKorpus.height() / 2) - widok,
		teksturaKorpus
	);
	
	QVector2D wektorZwrotKorpusuZnormalizowany(cos(this->zwrotKorpusu), sin(this->zwrotKorpusu));
	QVector2D wektorZwrotWiezyZnormalizowany(cos(this->zwrotKorpusu + this->zwrotWiezy), sin(this->zwrotKorpusu + this->zwrotWiezy));
	QVector2D wektorPrzesuniecieOsiWiezy(
		this->specyfikacja->przesuniecieOsiDlaKorpusu * -wektorZwrotKorpusuZnormalizowany.x() + this->specyfikacja->przesuniecieOsiDlaWiezy * wektorZwrotWiezyZnormalizowany.x(),
		this->specyfikacja->przesuniecieOsiDlaKorpusu * -wektorZwrotKorpusuZnormalizowany.y() + this->specyfikacja->przesuniecieOsiDlaWiezy * wektorZwrotWiezyZnormalizowany.y()
	);
	
	transformacja.rotateRadians(-this->zwrotWiezy);
	QPixmap teksturaWieza = this->specyfikacja->teksturaWieza.teksturaPrzeskalowana.transformed(transformacja);
	painter.drawPixmap(
		Obiekt::skala * this->pozycja + QPointF(Obiekt::skala * wektorPrzesuniecieOsiWiezy.x(), Obiekt::skala * -wektorPrzesuniecieOsiWiezy.y()) - QPoint(teksturaWieza.width() / 2, teksturaWieza.height() / 2) - widok,
		teksturaWieza
	);
	
	painter.setPen(Qt::red);
	painter.setBrush(Qt::red);
	painter.drawEllipse(Obiekt::skala * this->pozycja - widok, Obiekt::skala * 5, Obiekt::skala * 5);
}
