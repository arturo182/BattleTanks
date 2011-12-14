#ifndef OBIEKT_H
#define OBIEKT_H

#include <QPointF>
#include <QPainter>

class Obiekt{
	public:
		static double skala;
		QPointF pozycja;
		
		Obiekt(QPointF);
		virtual void rysuj(QPainter&) const = 0;
};

#endif // OBIEKT_H
