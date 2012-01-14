#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QGraphicsPathItem>

class Sciezka;

struct ObcyPojazd
{
		ObcyPojazd(): pojazd(-1), zwrot(0), pocisk(0) { }

	int pojazd;
	float zwrot;
	int pocisk;
};

class Waypoint : public QObject, public QGraphicsPathItem
{
	Q_OBJECT

	public:
		enum { Type = UserType + 2 };

		Waypoint(const QPointF &srodek);
		~Waypoint();

		int type() const { return Type; }
		QRectF boundingRect() const;

		void aktualizujKsztalt();

		void usunSciezke(Sciezka *sciezka);
		void usunSciezki();
		void dodajSciezke(Sciezka *sciezka);
		QList<Sciezka*> sciezki() const { return this->sciezkiWaypointu; }

		int id;
		ObcyPojazd *obcyPojazd;

	signals:
		void pozycjaZmieniona();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
		void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
		QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	private:
		QList<Sciezka*> sciezkiWaypointu;
		QPainterPath czolg;
		QPainterPath marker;
		bool uchwyt;
};

#endif // WAYPOINT_H
