#ifndef SCENA_H
#define SCENA_H

#include <QGraphicsScene>

class Scena : public QGraphicsScene
{
	Q_OBJECT
	Q_ENUMS(Tryby)
	Q_PROPERTY(Tryby tryb READ tryb WRITE ustawTryb)

	public:
		enum Tryby{
			ZAZNACZANIE = 0,
			PRZESUWANIE_WIDOKU,
			PRZESUWANIE_ELEMENTU,
			EDYCJA_WIERZCHOLKOW,
			DODAWANIE_PRZESZKODY,
			DODAWANIE_WAYPOINTU,
			LACZENIE_WAYPOINTOW
		};

		int przeszkody;
		int waypointy;

	public:
		Scena();

		class Przeszkoda *dodajPrzeszkode(const QPolygon &poly);
		class Waypoint *dodajWaypoint(const QPoint &punkt);
		class Sciezka *dodajSciezke(Waypoint *poczatek, Waypoint *koniec);

		Tryby tryb() const { return this->trybSceny; }
		void ustawTryb(Tryby tryb) { this->trybSceny = tryb; }

	signals:
		void trybZmieniony();
		void przeszkodaDodana();
		void waypointDodany();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	private:
		QGraphicsLineItem *linia;
		Tryby trybSceny;
};

#endif // SCENA_H
