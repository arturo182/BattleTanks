#include "przeszkoda.h"
#include "scena.h"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QPen>
#include <qmath.h>

Przeszkoda::Przeszkoda(const QPolygon &polygon):
QGraphicsPolygonItem(polygon),
id(0),
punktUchwytu(-1)
{
	this->setPen(QPen(Qt::red, 2));
	this->setBrush(QColor(255, 0, 0, 128));
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
}

QRectF Przeszkoda::boundingRect() const
{
	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW)
		return QGraphicsPolygonItem::boundingRect().adjusted(-5, -5, 5, 5);
	else
		return QGraphicsPolygonItem::boundingRect();
}

void Przeszkoda::setPolygon(const QPolygonF &polygon)
{
	QGraphicsPolygonItem::setPolygon(polygon);

	if(this->czyWypukly()) {
		this->setPen(QPen(Qt::red, 2));
		this->setBrush(QColor(255, 0, 0, 128));
	} else {
		this->setPen(QPen(QColor(255, 0, 255), 2));
		this->setBrush(QColor(255, 0, 255, 128));
	}
}

void Przeszkoda::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QGraphicsPolygonItem::paint(painter, option, widget);

	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW) {
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);

		foreach(QPoint wierzcholek, this->polygon().toPolygon()) {
			wierzcholek -= QPoint(5, 5);

			painter->drawRect(QRect(wierzcholek, QSize(10, 10)));
		}
	}
}

void Przeszkoda::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW) {
		QPointF pos = event->pos();
		this->punktUchwytu = this->uchwytNaPozycji(pos.toPoint());

		if(event->modifiers() & Qt::ControlModifier) {
			QPolygon poly = this->polygon().toPolygon();

			// usuwanie wierzcholka
			if(this->punktUchwytu > -1) {
				poly.remove(this->punktUchwytu);
				this->punktUchwytu = -1;

			// dodawanie wierzcholka
			} else {
				float minDyst = -1;
				int minWierz = 0;
				for(int i = 0; i < poly.size(); i++) {
					QPointF p1 = poly.at(i);
					QPointF p2 = poly.at((i+1) % poly.size());

					float dyst;

					if(p1 == p2)
						dyst = 0;
					else if(p1.y() == p2.y())
						dyst = qAbs(pos.y() - p1.y());
					else if (p1.x() == p2.x())
						dyst = qAbs(pos.x() - p1.x());
					else {
						float a = (p1.y() - p2.y()) / (p1.x() - p2.x());
						float c = (p1.x() * p2.y() - p1.y() * p2.x()) / (p1.x() - p2.x());

						dyst = qAbs(a * pos.x() -pos.y() + c) / qSqrt(a * a + 1);
					}

					if(minDyst < 0 || dyst < minDyst) {
						minDyst = dyst;
						minWierz = i;
					}
				}

				poly.insert(minWierz + 1, pos.toPoint());
				this->punktUchwytu = minWierz;
			}

			this->prepareGeometryChange();
			this->setPolygon(poly);
		}
	}

	QGraphicsPolygonItem::mousePressEvent(event);
}

void Przeszkoda::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint lastPos = event->lastPos().toPoint();
	QPoint pos = event->pos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW) {
		if(this->punktUchwytu > -1) {
			QPolygon poly = this->polygon().toPolygon();

			poly.setPoint(this->punktUchwytu, poly.point(this->punktUchwytu) + delta);

			this->prepareGeometryChange();
			this->setPolygon(poly);
			emit wierzcholekZmieniony();
		}
	} else if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		this->prepareGeometryChange();
		this->moveBy(delta.x(), delta.y());
		emit pozycjaZmieniona();
	}
}

void Przeszkoda::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPoint pos = event->pos().toPoint();

	this->setCursor(Qt::ArrowCursor);

	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW)
		if(this->uchwytNaPozycji(pos) > -1)
			this->setCursor(Qt::CrossCursor);

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		this->setCursor(Qt::SizeAllCursor);
}

int Przeszkoda::uchwytNaPozycji(const QPoint &pos)
{
	for(int i = 0; i < this->polygon().count(); i++) {
		QPoint wierzcholek = this->polygon().toPolygon().point(i);

		QRect uchwyt(wierzcholek - QPoint(5, 5), QSize(10, 10));
		if(uchwyt.contains(pos)) {
			return i;
		}
	}

	return -1;
}

bool Przeszkoda::czyWypukly()
{
	if(this->polygon().size() < 3)
		return false;

	QPointF p;
	QPointF v;
	QPointF u;

	int res = 0;
	for(int i = 0; i < this->polygon().size(); i++) {
		p = this->polygon().at(i);
		QPointF tmp = this->polygon().at((i+1) % this->polygon().size());
		v.setX(tmp.x() - p.x());
		v.setY(tmp.y() - p.y());
		u = this->polygon().at((i+2) % this->polygon().size());

		if(i == 0) {
			res = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
		} else {
			int newres = u.x() * v.y() - u.y() * v.x() + v.x() * p.y() - v.y() * p.x();
			if((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return false;
		}
	}

	return true;
}

void Przeszkoda::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->scene()->property("tryb").toInt() == Scena::EDYCJA_WIERZCHOLKOW)
		this->punktUchwytu = -1;

	QGraphicsPolygonItem::mouseReleaseEvent(event);
}
