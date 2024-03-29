#ifndef SCENA_H
#define SCENA_H

#include <QGraphicsScene>

class Przeszkoda;

class Scena : public QGraphicsScene
{
	Q_OBJECT
	Q_ENUMS(Tryb)
	Q_PROPERTY(Tryb tryb READ tryb WRITE ustawTryb)

	public:
		enum Tryb{
			ZAZNACZANIE = 0,
			PRZESUWANIE_WIDOKU,
			PRZESUWANIE_ELEMENTU,
			EDYCJA_WIERZCHOLKOW,
			DODAWANIE_PRZESZKODY,
			DODAWANIE_WAYPOINTU,
			LACZENIE_WAYPOINTOW,
			POZYCJA_GRACZA,
			OBCY_POJAZD,
			META_LABIRYNTU
		};

	public:
		Scena(const QRectF &prostokat = QRectF());

		class Przeszkoda *dodajPrzeszkode(const QPolygon &poly);
		class Waypoint *dodajWaypoint(const QPoint &punkt);
		class Sciezka *dodajSciezke(Waypoint *poczatek, Waypoint *koniec);
		class Gracz *dodajGracza(const QPointF &punkt);
		class Meta *dodajMete(const QPointF &punkt);

		Tryb tryb() const { return this->trybSceny; }
		void ustawTryb(Tryb tryb) { this->trybSceny = tryb; }

	signals:
		void trybZmieniony();
		void przeszkodaDodana();
		void waypointDodany();
		void sciezkaDodana();
		void elementPrzesuniety();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	private:
		QGraphicsLineItem *linia;
		Przeszkoda *przeszkoda;
		Tryb trybSceny;
};

#endif // SCENA_H
