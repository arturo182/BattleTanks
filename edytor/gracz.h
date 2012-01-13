#ifndef GRACZ_H
#define GRACZ_H

#include <QGraphicsPathItem>

class Gracz : public QObject, public QGraphicsPathItem
{
	Q_OBJECT

	public:
		enum { Type = UserType + 4 };

		Gracz(const QPointF &srodek);

		int type() const { return Type; }

		QRectF boundingRect() const;

	signals:
		void pozycjaZmieniona();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	private:
		bool uchwyt;
};

#endif // GRACZ_H
