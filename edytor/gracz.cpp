#include "gracz.h"

#include "scena.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <qmath.h>
#include <QCursor>
#include <QDebug>

Gracz::Gracz(const QPointF &srodek):
uchwyt(false)
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addRect(0, 0, 100, 70);
	path.addEllipse(QPointF(50, 35), 20, 20);
	path.addRect(70, 30, 60, 10);
	this->setPath(path);

	this->setPen(QPen(Qt::black, 2));
	this->setBrush(QColor(0, 0, 0, 128));
	this->setTransformOriginPoint(50, 35);
	this->setPos(srodek);
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
}

QRectF Gracz::boundingRect() const
{
	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		return QGraphicsPathItem::boundingRect().adjusted(0, 0, 3, 0);
	else
		return QGraphicsPathItem::boundingRect();
}

void Gracz::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastPos().toPoint();
	QPoint pos = event->pos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		if(uchwyt) {
			//rotacja
		} else {
			this->prepareGeometryChange();
			this->moveBy(delta.x(), delta.y());
		}

		emit pozycjaZmieniona();
	}
}

void Gracz::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint pos = event->pos().toPoint();
	QRect uchwyt(QPoint(127, 32), QSize(6, 6));

	this->uchwyt = uchwyt.contains(pos);
	QGraphicsPathItem::mousePressEvent(event);
}

void Gracz::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	this->uchwyt = false;
	QGraphicsPathItem::mouseReleaseEvent(event);
}

void Gracz::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPoint pos = event->pos().toPoint();

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		QRect uchwyt(QPoint(127, 32), QSize(6, 6));

		if(uchwyt.contains(pos))
			this->setCursor(QPixmap(":/ikony/rotate_icon.png"));
		else
			this->setCursor(Qt::SizeAllCursor);
	} else {
		this->setCursor(Qt::ArrowCursor);
	}
}

void Gracz::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsPathItem::paint(painter, option, widget);

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);

		painter->drawRect(QRect(QPoint(127, 32), QSize(6, 6)));
	}
}
