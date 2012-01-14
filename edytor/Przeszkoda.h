#ifndef PRZESZKODA_H
#define PRZESZKODA_H

#include <QGraphicsPolygonItem>

class Przeszkoda : public QObject, public QGraphicsPolygonItem
{
	Q_OBJECT

	public:
		enum { Type = UserType + 1 };

		Przeszkoda(const QPolygon &polygon);

		QRectF boundingRect() const;
		int type() const { return Type; }
		void setPolygon(const QPolygonF &polygon);

		int id;

	signals:
		void pozycjaZmieniona();
		void wierzcholekZmieniony();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

	private:
		int uchwytNaPozycji(const QPoint &pos);
		bool czyWypukly();

	private:
		int punktUchwytu;
};

#endif // PRZESZKODA_H
