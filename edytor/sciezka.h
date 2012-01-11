#ifndef SCIEZKA_H
#define SCIEZKA_H

#include <QGraphicsLineItem>

class Waypoint;

class Sciezka : public QGraphicsLineItem
{
	public:
		enum { Type = UserType + 3 };

		Sciezka(Waypoint *poczatek, Waypoint *koniec);

		int type() const { return Type; }

		QRectF boundingRect() const;

		Waypoint *poczatek() const { return this->elementPoczatkowy; }
		Waypoint *koniec() const { return this->elementKoncowy; }

		void aktualizujPozycje();

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	private:
		Waypoint *elementPoczatkowy;
		Waypoint *elementKoncowy;
};

#endif // SCIEZKA_H
