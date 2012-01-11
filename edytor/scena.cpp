#include "scena.h"
#include "przeszkoda.h"
#include "waypoint.h"

#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include <QDebug>
#include <qmath.h>

Scena::Scena():
QGraphicsScene(),
przeszkody(0),
waypointy(0)
{
}

void Scena::dodajPrzeszkode(const QPolygon &poly)
{
	this->przeszkody++;

	this->addItem(new Przeszkoda(poly));

	emit przeszkodaDodana();
}

void Scena::dodajWaypoint(const QPoint &punkt)
{
	this->waypointy++;

	this->addItem(new Waypoint(punkt));

	emit waypointDodany();
}

void Scena::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->property("tryb").toInt() == Scena::DODAWANIE_PRZESZKODY) {
		int boki = this->property("boki").toInt();

		QPointF srodek = event->scenePos();
		double r = 100.0;
		QPolygonF poly;
		float alfa;

		for(int i = 0; i < boki; i++) {
			alfa = 2.0 * M_PI * i / boki;
			poly.push_back(srodek + r * QPointF(qSin(alfa), -qCos(alfa)));
		}

		this->dodajPrzeszkode(poly.toPolygon());

		this->setProperty("tryb", Scena::ZAZNACZANIE);
		emit trybZmieniony();
	} else if(this->property("tryb").toInt() == Scena::DODAWANIE_WAYPOINTU) {
		QPoint srodek = event->scenePos().toPoint();

		qDebug() << srodek;
		this->dodajWaypoint(srodek);

		this->setProperty("tryb", Scena::ZAZNACZANIE);
		emit trybZmieniony();
	}

	QGraphicsScene::mousePressEvent(event);
}

