#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsPathItem>

class Sciezka;

class Waypoint : public QGraphicsPathItem
{
	public:
		enum { Type = UserType + 2 };

		Waypoint(const QPointF &srodek);

		int type() const { return Type; }

		void usunSciezke(Sciezka *sciezka);
		void usunSciezki();
		void dodajSciezke(Sciezka *sciezka);

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	private:
		QList<Sciezka*> sciezki;
};

#endif // WAYPOINT_H
