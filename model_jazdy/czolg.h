#ifndef CZOLG_H
#define CZOLG_H

#include <QPixmap>
#include <QPoint>
#include <QSize>

class Czolg{
	public:
		QPointF pozycja;
		QSize rozmiar;
		QPixmap korpus;
		QPixmap wieza;
		int przesuniecie_osi_wiezy_dla_korpusu;
		int przesuniecie_osi_wiezy_dla_wiezy;
		double kierunek;
		double kierunek_wiezy;
		double predkosc_maksymalna;
		double predkosc_obrotu_wieza;
		Czolg(QPointF, QPixmap, QPixmap, int, int, double, double, double);
};

#endif // CZOLG_H
