#ifndef PLANSZA_H
#define PLANSZA_H

#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include "czolg.h"
#include "pocisk.h"

class Plansza: public QLabel{
	Q_OBJECT

private:
	QTimer timer;
	QRect mapa;
	QLine* odcinki;
	int ilosc_odcinkow;
	QPoint widok;
	Czolg czolg;
	QList<Pocisk> pociski;
	int predkosc[2];
	int rotacja_wiezy;
	bool strzal;
	QRectF o;
	QRectF przemiesc(Czolg&, double, double, double);
	void rysuj_drazek(QPainter&, QRect, double);
	void rysuj_mape(QPainter&, QRect);
	
public:
	explicit Plansza(QWidget* = 0);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	void paintEvent(QPaintEvent*);

signals:

public slots:
	void ruch();

};

#endif // PLANSZA_H
