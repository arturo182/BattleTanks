#include "dotyk.h"
#include "silnik.h"

#include <QSwipeGesture>
#include <QDebug>

Dotyk::Dotyk():
	QObject(),
	nawigatorPolozenie(0x04),
	nawigatorWcisniete(0x04),
	nawig(0)
{
	for(size_t i = 0; i <= sizeof(this->przyciskiPolozenie); i++) {
		this->przyciski[i] = false;
		this->przyciskiPolozenie[i] = false;
		this->przyciskiWcisniete[i] = false;
	}

	this->dzojstiki[0] = 0.0f;
	this->dzojstiki[1] = 0.0f;

	this->sensor = new QAccelerometer(this);
	this->sensor->start();
}

bool Dotyk::otworz()
{
	return true;
}

void Dotyk::odswiez()
{
	if(this->silnik->tryb == Silnik::ROZGRYWKA) {
		QAccelerometerReading *reading = this->sensor->reading();

		qreal z = qAbs(reading->z());
		if(z > 17) {
			this->przyciski[PAUZA] = true;
		} else {
			this->przyciski[PAUZA] = false;
		}

		qreal x = reading->x() - 5.0f;
		qreal y = reading->y();

		y = qBound(-40.0f, y, 40.0f) / 40.0f;
		x = qBound(-3.0f, x, 3.0f) / 3.0f;

		x = qRound(x * 10.0f) / 10.0f;
		y = qRound(y * 10.0f) / 10.0f;

		qreal lewa = 0;
		qreal prawa = 0;

		if(y < 0) {
			prawa = x;
			lewa = y + 1;
		} else if(y > 0) {
			prawa = y;
			lewa = x;
		} else if(y == 0) {
			prawa = x;
			lewa = x;
		}

		this->dzojstiki[0] = lewa;
		this->dzojstiki[1] = prawa;
	}

	for(size_t i = 0; i <= sizeof(this->przyciskiPolozenie); i++) {
		bool polozenie = this->przyciski[i];
		this->przyciskiWcisniete[i] = polozenie && !this->przyciskiPolozenie[i];
		this->przyciskiPolozenie[i] = polozenie;
	}

	int status = this->nawig;
	this->nawigatorWcisniete = status & ~this->nawigatorPolozenie;
	this->nawigatorPolozenie = status;
}

void Dotyk::poke(QTouchEvent *event)
{
	if(event->type() == QEvent::TouchBegin) {
		foreach(QTouchEvent::TouchPoint point, event->touchPoints()) {
			QPointF pos = point.screenPos();

			if(this->silnik->tryb == Silnik::MENU || this->silnik->tryb == Silnik::PAUZA) {
				const QRectF strzalkaLewo(10, 5, 75, 75);
				const QRectF strzalkaPrawo(10, 95, 75, 75);
				const QRectF strzalkaGora(10, 185, 75, 75);
				const QRectF strzalkaDol(10, 275, 75, 75);
				const QRectF przycisk1(560, 5, 75, 75);
				const QRectF przycisk2(560, 95, 75, 75);
				const QRectF przycisk3(560, 185, 75, 75);
				const QRectF przycisk4(560, 275, 75, 75);

				if(strzalkaLewo.contains(pos)) {
					this->nawig |= LEWO;
				} else if(strzalkaPrawo.contains(pos)) {
					this->nawig |= PRAWO;
				} else if(strzalkaGora.contains(pos)) {
					this->nawig |= GORA;
				} else if(strzalkaDol.contains(pos)) {
					this->nawig |= DOL;
				} else if(przycisk1.contains(pos)) {
					this->przyciski[USUN] = true;
				} else if(przycisk2.contains(pos)) {
					this->przyciski[WYBIERZ] = true;
				} else if(przycisk3.contains(pos)) {
					this->przyciski[WYBIERZ_ALT] = true;
				} else if(przycisk4.contains(pos)) {
					this->przyciski[COFNIJ] = true;
				}
			} else if(this->silnik->tryb == Silnik::ROZGRYWKA) {
				QRectF wiezaLewo(5, 280, 75, 75);
				QRectF wiezaPrawo(560, 280, 75, 75);

				QRectF celownikDalej(85, 280, 75, 75);
				QRectF celownikBlizej(480, 280, 75, 75);

				QRectF bron(5, 200, 75, 75);
				QRectF strzal(560, 200, 75, 75);

				if(wiezaLewo.contains(pos)) {
					this->nawig |= LEWO;
				} else if(wiezaPrawo.contains(pos)) {
					this->nawig |= PRAWO;
				} else if(celownikDalej.contains(pos)) {
					this->nawig |= GORA;
				} else if(celownikBlizej.contains(pos)) {
					this->nawig |= DOL;
				} else if(bron.contains(pos)) {
					this->przyciski[BRON_PLUS] = true;
				} else if(strzal.contains(pos)) {
					this->przyciski[WYSTRZAL] = true;
				}
			}
		}
	} else if(event->type() == QEvent::TouchUpdate) {
		foreach(QTouchEvent::TouchPoint point, event->touchPoints()) {
			QPointF pos = point.screenPos();
			QPointF lastPos = point.lastScreenPos();

			if(this->silnik->tryb == Silnik::MENU || this->silnik->tryb == Silnik::PAUZA) {
				int xDist = lastPos.x() - pos.x();
				int yDist = lastPos.y() - pos.y();

				if(xDist > 50) {
					this->nawig |= LEWO;
				} else if(xDist < -50) {
					this->nawig |= PRAWO;
				} else if(yDist > 50) {
					this->nawig |= GORA;
				} else if(yDist < -50) {
					this->nawig |= DOL;
				}
			}
		}
	} else if(event->type() == QEvent::TouchEnd) {
		this->nawig = 0;

		for(size_t i = 0; i <= sizeof(this->przyciskiPolozenie); i++) {
			this->przyciski[i] = false;
		}
	}
}

void Dotyk::miziu(QGestureEvent *event)
{
	if(QGesture *gest = event->gesture(Qt::SwipeGesture)) {
		QSwipeGesture *swipe = static_cast<QSwipeGesture*>(gest);
		if(swipe->horizontalDirection() == QSwipeGesture::Left) {
			this->nawig |= LEWO;
		} else if(swipe->horizontalDirection() == QSwipeGesture::Right) {
			this->nawig |= PRAWO;
		} else if(swipe->verticalDirection() == QSwipeGesture::Up) {
			this->nawig |= GORA;
		} else if(swipe->verticalDirection() == QSwipeGesture::Down) {
			this->nawig |= DOL;
		}
	}
}

bool Dotyk::statusPrzyciskPolozenie(UrzadzenieWejscia::Przycisk przycisk) const
{
	if(przycisk <= (int)sizeof(this->przyciskiPolozenie))
		return this->przyciskiPolozenie[przycisk];

	return false;
}

bool Dotyk::statusPrzyciskWcisniecie(UrzadzenieWejscia::Przycisk przycisk) const
{
	if(przycisk <= (int)sizeof(this->przyciskiWcisniete)) {
		return this->przyciskiWcisniete[przycisk];
	}

	return false;
}

int Dotyk::statusNawigatorPolozenie() const
{
	return this->nawigatorPolozenie;
}

int Dotyk::statusNawigatorWcisniecie() const
{
	return this->nawigatorWcisniete;
}

float Dotyk::statusDzojstik(UrzadzenieWejscia::Dzojstik dzojstik) const
{
	if(dzojstik == LEWY_PIONOWY)
		return this->dzojstiki[0];
	else if(dzojstik == PRAWY_PIONOWY)
		return this->dzojstiki[1];

	return 0.0f;
}
