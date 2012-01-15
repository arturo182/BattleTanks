#ifndef META_H
#define META_H

#include <QGraphicsPathItem>

class Meta : public QObject, public QGraphicsPathItem
{
	Q_OBJECT

	public:
		enum { Type = UserType + 5 };

		Meta(const QPointF &pozycja);

		int type() const { return Type; }

	signals:
		void pozycjaZmieniona();

	protected:
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

};

#endif // META_H
