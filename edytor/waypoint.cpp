#include "waypoint.h"
#include "sciezka.h"
#include "scena.h"

#include <QGraphicsSceneMouseEvent>
#include <QCursor>

Waypoint::Waypoint(const QPointF &srodek):
QGraphicsPathItem()
{
	int rozmiar = 60;

	QPainterPath path;

	path.quadTo(-(rozmiar * 0.5), -rozmiar, 0, -rozmiar);
	path.quadTo((rozmiar * 0.5), -rozmiar, 0, 0);
	path.addEllipse(-rozmiar * 0.125, -rozmiar * 0.85, rozmiar * 0.25, rozmiar * 0.25);

	this->setPath(path);
	this->setPen(QPen(QColor(0, 128, 0), 2));
	this->setBrush(QColor(0, 128, 0, 128));
	this->setPos(srodek);
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
	this->setZValue(100);
}

void Waypoint::usunSciezke(Sciezka *sciezka)
{
	this->sciezkiWaypointu.removeAll(sciezka);
}

void Waypoint::usunSciezki()
{
	foreach(Sciezka *sciezka, this->sciezkiWaypointu) {
		sciezka->poczatek()->usunSciezke(sciezka);
		sciezka->koniec()->usunSciezke(sciezka);

		//this->scene()->removeItem(sciezka);

		delete sciezka;
	}
}

void Waypoint::dodajSciezke(Sciezka *sciezka)
{
	this->sciezkiWaypointu.append(sciezka);
}

void Waypoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastPos().toPoint();
	QPoint pos = event->pos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		this->moveBy(delta.x(), delta.y());
		emit pozycjaZmieniona();
	}
}

void Waypoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPoint pos = event->pos().toPoint();

	this->setCursor(Qt::ArrowCursor);

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		this->setCursor(Qt::SizeAllCursor);

}

QVariant Waypoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionChange)
		foreach(Sciezka *sciezka, this->sciezkiWaypointu)
			sciezka->aktualizujPozycje();

	return value;
}
