#ifndef PLANSZA_H
#define PLANSZA_H

#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QSound>
#include <SDL/SDL.h>
#include "czolg.h"
#include "pocisk.h"
#include "tekstury.h"
#include "animacja.h"

class Plansza: public QLabel{
	Q_OBJECT

private:
	SDL_Joystick* joystick;
	QTimer timer;
	QPixmap& mapa;
	
	QPolygonF* obiekty;
	int ilosc_obiektow;
	
	QPoint widok;
	Czolg czolg;
	QList<Pocisk> pociski;
	double predkosc[2];
	int rotacja_wiezy;
	
	double predkosc_lufy;
	int podnoszenie_lufy;
	double cel_odleglosc;
	bool strzal;
	
	Tekstury* tekstury;
	QList<Animacja> eksplozje;
	QList<Animacja> bonusy;
	
	QList<QSound*> dzwieki;
	
	QRectF o;
	double zwrot(QPointF, QPointF, QPointF);
	QRectF przemiesc(Czolg&, double, double, double);
	void rysuj_drazek(QPainter&, QRect, double);
	void rysuj_mape(QPainter&, QRect);
	
public:
	explicit Plansza(QPixmap&, QWidget* = 0);
	virtual ~Plansza();
	void paintEvent(QPaintEvent*);

signals:

public slots:
	void ruch();

};

#endif // PLANSZA_H
