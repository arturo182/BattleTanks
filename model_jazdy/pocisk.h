#ifndef POCISK_H
#define POCISK_H

#include <QPoint>
#include <QVector2D>

class Pocisk{
	public:
		QPointF pozycja;
		QVector2D kierunek;
		double predkosc;
		
		Pocisk(QPointF, QVector2D, double);
};

#endif // POCISK_H
