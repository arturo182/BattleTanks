#include "scena.h"

#include "przeszkoda.h"
#include "waypoint.h"
#include "sciezka.h"

#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include <QDebug>
#include <qmath.h>

Scena::Scena():
QGraphicsScene(),
przeszkody(0),
waypointy(0),
linia(0)
{
}

Przeszkoda *Scena::dodajPrzeszkode(const QPolygon &poly)
{
	this->przeszkody++;

	Przeszkoda *przeszkoda = new Przeszkoda(poly);
	connect(przeszkoda, SIGNAL(pozycjaZmieniona()), SIGNAL(elementPrzesuniety()));
	this->addItem(przeszkoda);

	emit przeszkodaDodana();

	return przeszkoda;
}

Waypoint *Scena::dodajWaypoint(const QPoint &punkt)
{
	this->waypointy++;

	Waypoint *waypoint = new Waypoint(punkt);
	connect(waypoint, SIGNAL(pozycjaZmieniona()), SIGNAL(elementPrzesuniety()));
	this->addItem(waypoint);

	emit waypointDodany();

	return waypoint;
}

Sciezka *Scena::dodajSciezke(Waypoint *poczatek, Waypoint *koniec)
{
	Sciezka *sciezka = new Sciezka(poczatek, koniec);

	poczatek->dodajSciezke(sciezka);
	koniec->dodajSciezke(sciezka);

	sciezka->aktualizujPozycje();

	this->addItem(sciezka);
	emit sciezkaDodana();

	return sciezka;
}

void Scena::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->trybSceny == Scena::DODAWANIE_PRZESZKODY) {
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
	} else if(this->trybSceny == Scena::DODAWANIE_WAYPOINTU) {
		this->dodajWaypoint(event->scenePos().toPoint());
	} else if(this->trybSceny ==  Scena::LACZENIE_WAYPOINTOW) {
		this->linia = new QGraphicsLineItem(QLineF(event->scenePos(),  event->scenePos()));
		this->linia->setPen(QPen(Qt::green, 2));
		this->addItem(linia);
	}

	QGraphicsScene::mousePressEvent(event);
}

void Scena::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->trybSceny == Scena::LACZENIE_WAYPOINTOW)
		if(this->linia != 0)
			this->linia->setLine(QLineF(this->linia->line().p1(), event->scenePos()));

	QGraphicsScene::mouseMoveEvent(event);
}

void Scena::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(this->linia != 0 && this->trybSceny == Scena::LACZENIE_WAYPOINTOW) {
		 QList<QGraphicsItem *> elementyPoczatkowe = this->items(this->linia->line().p1());
		 if(elementyPoczatkowe.count() && elementyPoczatkowe.first() == this->linia)
			 elementyPoczatkowe.removeFirst();

		 QList<QGraphicsItem *> elementyKoncowe = this->items(this->linia->line().p2());
		 if(elementyKoncowe.count() && elementyKoncowe.first() == this->linia)
			 elementyKoncowe.removeFirst();

		 this->removeItem(linia);
		 delete linia;

		 if(elementyPoczatkowe.count() && elementyKoncowe.count()) {
			 if((elementyPoczatkowe.first()->type() == Waypoint::Type) && (elementyKoncowe.first()->type() == Waypoint::Type)) {
				 if(elementyPoczatkowe.first() != elementyKoncowe.first()) {
					 Waypoint *poczatek = qgraphicsitem_cast<Waypoint*>(elementyPoczatkowe.first());
					 Waypoint *koniec = qgraphicsitem_cast<Waypoint*>(elementyKoncowe.first());

					 bool istnieje = false;
					 foreach(Sciezka *sciezka, poczatek->sciezki()) {
						 if(((sciezka->poczatek() == poczatek) && (sciezka->koniec() == koniec)) ||
							((sciezka->poczatek() == koniec) && (sciezka->koniec() == poczatek))) {
							 istnieje = true;
							 break;
						 }
					 }

					 if(!istnieje)
						this->dodajSciezke(poczatek, koniec);
				 }
			 }
		 }
	 }

	 this->linia = 0;

	 QGraphicsScene::mouseReleaseEvent(event);
}

void Scena::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	if(event->modifiers() & Qt::ControlModifier) {
		if(event->delta() > 0)
			emit zoomPrzyblizony();
		else if(event->delta() < 0)
			emit zoomOddalony();
	}

	QGraphicsScene::wheelEvent(event);
}
