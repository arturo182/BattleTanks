#ifndef POCISK_H
#define POCISK_H

#include <QVector2D>
#include "obiekt.h"
#include "tekstura.h"

class SpecyfikacjaPocisku;

class Pocisk: public Obiekt{
	private:
		const SpecyfikacjaPocisku* specyfikacja;
		Tekstura* tekstura;
		bool status;
		bool pociskGracza;
		QPointF punktStartowy;
		QVector2D wektorKierunku;
		float dystansCalkowity;
		float dystansAktualny;
		int czasMilisekundy;

	public:
		Pocisk(SpecyfikacjaPocisku*, bool, QPointF, QVector2D, float);
		~Pocisk();
		void odswiez(int);
		inline bool sprawdzStatus() const;
		void rysuj(QPainter&, QPoint) const;

	friend class Logika;
};

inline bool Pocisk::sprawdzStatus() const{
	return this->status;
}

#endif // POCISK_H
