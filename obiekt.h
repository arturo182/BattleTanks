#ifndef OBIEKT_H
#define OBIEKT_H

#include <QPointF>
#include <QPainter>

class Obiekt{
	public:
		static float skala;
		QPointF pozycja;
		
		Obiekt(QPointF);
		virtual ~Obiekt();
		virtual void rysuj(QPainter&, QPoint) const = 0;
};

#endif // OBIEKT_H
