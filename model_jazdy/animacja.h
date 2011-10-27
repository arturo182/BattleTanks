#ifndef ANIMACJA_H
#define ANIMACJA_H

#include <QPointF>
#include <QTime>
#include <QPixmap>
#include <QPainter>
#include "tekstury.h"

class Animacja{
	private:
		Tekstury& tekstury;
		QTime czas_rozpoczecia;
		int czas_trwania;
		bool powtarzaj;
		bool aktywna;
	
	protected:
		void rysuj_klatke(QPainter&, QPoint, QRect, int) const;
	
	public:
		const QPoint pozycja;
		
		Animacja(QPoint, Tekstury&, QTime, int, bool = false);
		bool rysuj(QPainter&, QPoint, QRect, QTime);
		void zakoncz();
};

inline void Animacja::zakoncz(){
	this->aktywna = false;
}

#endif // ANIMACJA_H
