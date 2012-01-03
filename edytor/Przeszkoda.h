#ifndef PRZESZKODA_H
#define PRZESZKODA_H

#include <QGraphicsPolygonItem>

class Przeszkoda : public QGraphicsPolygonItem
{
	public:
		enum { Type = UserType + 1 };

		Przeszkoda(const QPolygon &polygon);

		QRectF boundingRect() const;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	private:
		int uchwytNaPozycji(const QPoint &pos);

	private:
		int punktUchwytu;
};

#endif // PRZESZKODA_H