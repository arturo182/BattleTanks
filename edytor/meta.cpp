#include "meta.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QDebug>

#include "scena.h"

Meta::Meta(const QPointF &pozycja)
{
	QPainterPath path;
	path.addEllipse(QPointF(), 50, 50);
	path.addRect(-1, -1, 1, 1);
	this->setPath(path);

	this->setPen(QPen(Qt::yellow, 2));
	this->setBrush(QColor(255, 255, 0, 128));
	this->setPos(pozycja);
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
}

void Meta::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastScenePos().toPoint();
	QPoint pos = event->scenePos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		this->prepareGeometryChange();
		this->moveBy(delta.x(), delta.y());

		emit pozycjaZmieniona();
	}
}

void Meta::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		this->setCursor(Qt::SizeAllCursor);
	else
		this->setCursor(Qt::ArrowCursor);
}
