#ifndef OBIEKT_H
#define OBIEKT_H

#include <QPointF>
#include <QPainter>

class Obiekt{
	protected:
		QPointF pozycja;
		
	public:
		Obiekt();
		virtual void rysuj(QPainter&) const = 0;
};

#endif // OBIEKT_H
