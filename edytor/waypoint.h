#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsPathItem>

class Waypoint : public QGraphicsPathItem
{
	public:
		enum { Type = UserType + 2 };

		Waypoint(const QPointF &srodek);

		int type() const { return Type; }

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

};

#endif // WAYPOINT_H
