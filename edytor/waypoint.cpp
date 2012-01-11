#include "waypoint.h"
#include "scena.h"

#include <QGraphicsSceneMouseEvent>
#include <QCursor>

Waypoint::Waypoint(const QPointF &srodek):
QGraphicsPathItem()
{
	int rozmiar = 20;

	QPainterPath path;
	path.moveTo(-rozmiar, -rozmiar);
	path.lineTo( rozmiar,  rozmiar);
	path.moveTo(-rozmiar,  rozmiar);
	path.lineTo( rozmiar, -rozmiar);
	path.addEllipse(QPointF(), rozmiar * 0.5, rozmiar * 0.5);

	this->setPath(path);
	this->setPen(QPen(Qt::green, 2));
	this->setBrush(QColor(0, 255, 0, 128));
	this->setPos(srodek);
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
}

void Waypoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastPos().toPoint();
	QPoint pos = event->pos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		this->moveBy(delta.x(), delta.y());
}

void Waypoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPoint pos = event->pos().toPoint();

	this->setCursor(Qt::ArrowCursor);

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		this->setCursor(Qt::SizeAllCursor);

}
