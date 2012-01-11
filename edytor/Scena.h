#ifndef SCENA_H
#define SCENA_H

#include <QGraphicsScene>

class Scena : public QGraphicsScene
{
	Q_OBJECT

	public:
		enum Tryby{
			ZAZNACZANIE = 0,
			PRZESUWANIE_WIDOKU,
			PRZESUWANIE_ELEMENTU,
			EDYCJA_WIERZCHOLKOW,
			DODAWANIE_PRZESZKODY,
			DODAWANIE_WAYPOINTU
		};

		int przeszkody;
		int waypointy;

	public:
		Scena();

		void dodajPrzeszkode(const QPolygon &poly);
		void dodajWaypoint(const QPoint &punkt);

	signals:
		void trybZmieniony();
		void przeszkodaDodana();
		void waypointDodany();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // SCENA_H
