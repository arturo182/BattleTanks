#include "sciezka.h"
#include "waypoint.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QPen>

Sciezka::Sciezka(Waypoint *poczatek, Waypoint *koniec):
QGraphicsLineItem(),
elementPoczatkowy(poczatek),
elementKoncowy(koniec)
{
	this->setFlag(QGraphicsItem::ItemIsSelectable, true);
	this->setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Sciezka::boundingRect() const
{
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

void Sciezka::aktualizujPozycje()
{
	this->setLine(QLineF(this->mapFromItem(this->elementPoczatkowy, 0, 0), this->mapFromItem(this->elementKoncowy, 0, 0)));
}

void Sciezka::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(this->elementPoczatkowy->collidesWithItem(this->elementKoncowy))
		return;

	painter->setPen(this->pen());
	this->setLine(QLineF(this->elementPoczatkowy->pos(), this->elementKoncowy->pos()));

	painter->drawLine(this->line());

	if(this->isSelected()) {
		painter->setPen(QPen(Qt::black, 1, Qt::DashLine));

		QLineF myLine = this->line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);

		myLine.translate(0,-8.0);
		painter->drawLine(myLine);
	}
}
