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
	path.addRect(-50, -35, 100, 70);
	path.addEllipse(QPointF(), 20, 20);
	path.addRect(20, -5, 60, 10);
	this->setPath(path);

	this->setPen(QPen(Qt::black, 2));
	this->setBrush(QColor(0, 0, 0, 128));
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
	QPoint lastPos = event->lastScenePos().toPoint();
	QPoint pos = event->scenePos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		if(this->uchwyt) {
			int kat = qRound(qAtan2(this->pos().x() - event->scenePos().x(), this->pos().y() - event->scenePos().y()) * (180 / M_PI));
			kat += 90;

			if(kat < 0)
				kat += 360;

			if(!(event->modifiers() & Qt::ShiftModifier) || (kat % 30 == 0))
				this->setRotation(-kat);
		} else {
			this->prepareGeometryChange();
			this->moveBy(delta.x(), delta.y());
		}

		emit pozycjaZmieniona();
	}
}

void Gracz::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPathItem::mousePressEvent(event);

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		QPoint pos = event->pos().toPoint();
		QRect uchwyt(QPoint(77, -3), QSize(6, 6));

		this->uchwyt = uchwyt.contains(pos);
	}
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
		QRect uchwyt(QPoint(77, -3), QSize(6, 6));

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

		painter->drawRect(QRect(QPoint(77, -3), QSize(6, 6)));
	}
}
