#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsPathItem>

class Sciezka;

class Waypoint : public QObject, public QGraphicsPathItem
{
	Q_OBJECT

	public:
		enum { Type = UserType + 2 };

		Waypoint(const QPointF &srodek);

		int type() const { return Type; }

		void usunSciezke(Sciezka *sciezka);
		void usunSciezki();
		void dodajSciezke(Sciezka *sciezka);
		QList<Sciezka*> sciezki() const { return this->sciezkiWaypointu; }

		int id;

	signals:
		void pozycjaZmieniona();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	private:
		QList<Sciezka*> sciezkiWaypointu;
};

#endif // WAYPOINT_H
