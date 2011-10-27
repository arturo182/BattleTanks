#ifndef POCISK_H
#define POCISK_H

#include <QPointF>
#include <QTime>

class Pocisk{
	private:
		QPointF pozycja_poczatkowa;
		QPointF pozycja_docelowa;
		QPointF pozycja_aktualna;
		double predkosc;
		QTime czas_wystrzalu;
		double dystans_calkowity;
		
	public:
		Pocisk(QPointF, QPointF, double, QTime);
		bool przesun(QTime);
		const QPointF pozycja() const;
};

inline const QPointF Pocisk::pozycja() const{
	return this->pozycja_aktualna;
}

#endif // POCISK_H
