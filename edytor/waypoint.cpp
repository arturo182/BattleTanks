#include "waypoint.h"
#include "oknopojazduobcego.h"
#include "sciezka.h"
#include "scena.h"

#include <QGraphicsSceneMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <qmath.h>
#include <QCursor>
#include <QDebug>

Waypoint::Waypoint(const QPointF &srodek):
QGraphicsPathItem(),
id(0),
obcyPojazd(new ObcyPojazd()),
uchwyt(false)
{
	const int rozmiar = 60;

	this->marker.quadTo(-(rozmiar * 0.5), -rozmiar, 0, -rozmiar);
	this->marker.quadTo((rozmiar * 0.5), -rozmiar, 0, 0);
	this->marker.addEllipse(-rozmiar * 0.125, -rozmiar * 0.85, rozmiar * 0.25, rozmiar * 0.25);

	this->czolg.setFillRule(Qt::WindingFill);
	this->czolg.addRect(-50, -35, 100, 70);
	this->czolg.addEllipse(QPointF(), 20, 20);
	this->czolg.addRect(20, -5, 60, 10);

	this->setPath(this->marker);
	this->setPos(srodek);
	this->setPen(QPen(QColor(0, 128, 0), 2));
	this->setBrush(QColor(0, 128, 0, 128));
	this->setFlags(QGraphicsItem::ItemIsSelectable);
	this->setAcceptHoverEvents(true);
	this->setZValue(100);
}

Waypoint::~Waypoint()
{
	delete this->obcyPojazd;
}

QRectF Waypoint::boundingRect() const
{
	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU)
		return QGraphicsPathItem::boundingRect().adjusted(0, 0, 3, 0);
	else
		return QGraphicsPathItem::boundingRect();
}

void Waypoint::aktualizujKsztalt()
{
	this->prepareGeometryChange();
	if(this->obcyPojazd->pojazd > -1) {
		this->setPath(this->marker + this->czolg);
		this->setRotation(-(this->obcyPojazd->zwrot * (180 / M_PI)));
	} else {
		this->setPath(this->marker);
		this->obcyPojazd->zwrot = 0;
		this->setRotation(0);
	}

	update();
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
	QPoint lastPos = event->lastScenePos().toPoint();
	QPoint pos = event->scenePos().toPoint();
	QPoint delta = pos - lastPos;

	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		if(this->uchwyt) {
			int kat = qRound(qAtan2(this->pos().x() - event->scenePos().x(), this->pos().y() - event->scenePos().y()) * (180 / M_PI));
			kat += 90;

			if(kat < 0)
				kat += 360;

			if(!(event->modifiers() & Qt::ShiftModifier) || (kat % 30 == 0)) {
				this->setRotation(-kat);
				this->obcyPojazd->zwrot = kat * (M_PI / 180);
			}
		} else {
			this->moveBy(delta.x(), delta.y());
		}

		emit pozycjaZmieniona();
	}
}

void Waypoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		QPoint pos = event->pos().toPoint();

		QRect uchwyt(QPoint(77, -3), QSize(6, 6));

		if(uchwyt.contains(pos))
			this->setCursor(QPixmap(":/ikony/rotate_icon.png"));
		else
			this->setCursor(Qt::SizeAllCursor);
	} else {
		this->setCursor(Qt::ArrowCursor);
	}
}

QVariant Waypoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if(change == QGraphicsItem::ItemPositionChange)
		foreach(Sciezka *sciezka, this->sciezkiWaypointu)
			sciezka->aktualizujPozycje();

	return value;
}

void Waypoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	const int rozmiar = 60;

	if(this->obcyPojazd->pojazd > -1) {
		painter->setPen(QPen(Qt::black, 2));
		painter->setBrush(QColor(0, 0, 0, 128));
		painter->drawPath(this->czolg);

		if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
			painter->setPen(Qt::black);
			painter->setBrush(Qt::white);

			painter->drawRect(QRect(QPoint(77, -3), QSize(6, 6)));
		}
	}

	if(option->state & QStyle::State_Selected) {
		const qreal margines = painter->pen().widthF() / 2;

		const QColor kolorTekstu = option->palette.windowText().color();
		const QColor kolor(kolorTekstu.red() > 127 ? 0 : 255, kolorTekstu.green() > 127 ? 0 : 255, kolorTekstu.blue() > 127 ? 0 : 255);

		painter->setPen(QPen(kolor, 0, Qt::SolidLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(this->boundingRect().adjusted(margines, margines, -margines, -margines));

		painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(this->boundingRect().adjusted(margines, margines, -margines, -margines));
	}

	painter->rotate(-this->rotation());
	painter->setPen(this->pen());
	painter->setBrush(this->brush());
	painter->drawPath(this->marker);

	QFont f = painter->font();
	f.setPointSize(6);
	painter->setFont(f);
	painter->setPen(QColor(0, 128, 0));
	painter->drawText(QRectF(-rozmiar * 0.250, -rozmiar * 0.975, rozmiar * 0.5, rozmiar * 0.5), QString::number(this->id), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

}

void Waypoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsPathItem::mousePressEvent(event);

	if(this->scene()->property("tryb").toInt() == Scena::OBCY_POJAZD) {
		OknoPojazduObcego okno(this->obcyPojazd->pojazd, this->obcyPojazd->pocisk);
		if(okno.exec() == QDialog::Accepted) {
			this->obcyPojazd->pojazd = okno.obcyPojazd.pojazd;
			this->obcyPojazd->pocisk = okno.obcyPojazd.pocisk;
			qDebug() << this->obcyPojazd->pocisk;
			this->aktualizujKsztalt();
		}
	} else if(this->scene()->property("tryb").toInt() == Scena::PRZESUWANIE_ELEMENTU) {
		QPoint pos = event->pos().toPoint();
		QRect uchwyt(QPoint(77, -3), QSize(6, 6));

		this->uchwyt = uchwyt.contains(pos);
	}
}

void Waypoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	this->uchwyt = false;
	QGraphicsPathItem::mouseReleaseEvent(event);
}
