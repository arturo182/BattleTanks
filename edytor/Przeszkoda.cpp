#include "Przeszkoda.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>
#include <QPen>

Przeszkoda::Przeszkoda(const QPolygon &polygon):
	QGraphicsPolygonItem(polygon),
	punktUchwytu(-1)
{
	this->setPen(QPen(Qt::red, 2));
	this->setBrush(QColor(255, 0, 0, 128));
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
}

QRectF Przeszkoda::boundingRect() const
{
	return QGraphicsPolygonItem::boundingRect().adjusted(-3, -3, 3, 3);
}

void Przeszkoda::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsPolygonItem::paint(painter, option, widget);

	painter->setPen(Qt::black);
	painter->setBrush(Qt::white);

	foreach(QPoint wierzcholek, this->polygon().toPolygon()) {
		wierzcholek -= QPoint(3, 3);

		painter->drawRect(QRect(wierzcholek, QSize(6, 6)));
	}
}

void Przeszkoda::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint pos = event->scenePos().toPoint();
	this->punktUchwytu = this->uchwytNaPozycji(pos);

	QGraphicsPolygonItem::mousePressEvent(event);
}

void Przeszkoda::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastScenePos().toPoint();
	QPoint pos = event->scenePos().toPoint();
	QPoint delta = pos - lastPos;


	if(this->punktUchwytu > -1) {
		QPolygon poly = this->polygon().toPolygon();

		poly.setPoint(this->punktUchwytu, poly.point(this->punktUchwytu) + delta);

		this->setPolygon(poly);
	} else {
		if(this->punktUchwytu == -1) {
			QPointF p = this->pos();

			p += delta;

			this->setPos(p);
		}
	}
}

void Przeszkoda::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPoint pos = event->scenePos().toPoint();

	if(this->uchwytNaPozycji(pos) > -1)
		this->setCursor(Qt::CrossCursor);
	else
		this->setCursor(Qt::SizeAllCursor);
}

int Przeszkoda::uchwytNaPozycji(const QPoint &pos)
{
	for(int i = 0; i < this->polygon().count(); i++) {
		QPoint wierzcholek = this->polygon().toPolygon().point(i);

		QRect uchwyt(wierzcholek - QPoint(3, 3), QSize(6, 6));
		if(uchwyt.contains(pos)) {
			return i;
		}
	}

	return -1;
}

void Przeszkoda::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	this->punktUchwytu = -1;

	QGraphicsPolygonItem::mouseReleaseEvent(event);
}
