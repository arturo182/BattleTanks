#include <cmath>
#include <QVector2D>
#include <QDebug>
#include "plansza.h"

Plansza::Plansza(QWidget* parent):
	QLabel(parent),
	mapa(QRect(0, 0, 3000, 3000)),
	widok(QPoint(0, 0)),
	czolg(QPointF(100, 100), QPixmap(":/tank_korpus.png"), QPixmap(":/tank_wieza.png"), -10, 23, 0, 10, M_PI_2),
	rotacja_wiezy(0),
	strzal(false),
	o(0, 0, 0, 0){
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(ruch()));
	this->resize(800, 600);
	this->ilosc_odcinkow = 1;
	this->odcinki = new QLine[this->ilosc_odcinkow];
	this->odcinki[0] = QLine(500, 100, 550, 700);
	this->predkosc[0] = this->predkosc[1] = 0;
	this->timer.start(20);
}

void Plansza::keyPressEvent(QKeyEvent* e){
/*	int keys[] = {90, 65, 47, 39}, i = 0;
	while(i < 4 && keys[i] != e->key())
		i++;
	if(i < 4 && (i % 2 ? (this->predkosc[i / 2] < 10) : (this->predkosc[i / 2] > -10)))
		this->predkosc[i / 2] += 2 * (i % 2) - 1;*/
		
	switch(e->key()){
		case 65:
			if(this->predkosc[0] < 10)
				this->predkosc[0]++;
			break;
		case 90:
			if(this->predkosc[0] > -10)
				this->predkosc[0]--;
			break;
		case 70:
			if(this->predkosc[1] < 10)
				this->predkosc[1]++;
			break;
		case 67:
			if(this->predkosc[1] > -10)
				this->predkosc[1]--;
			break;
		case Qt::Key_Left:
			this->rotacja_wiezy = 1;
			break;
		case Qt::Key_Right:
			this->rotacja_wiezy = -1;
			break;
		case Qt::Key_Up:
			this->strzal = true;
			break;
	}
}

void Plansza::keyReleaseEvent(QKeyEvent* e){
	if((e->key() == Qt::Key_Left && this->rotacja_wiezy > 0) || (e->key() == Qt::Key_Right && this->rotacja_wiezy < 0))
		this->rotacja_wiezy = 0;
}

QRectF Plansza::przemiesc(Czolg& czolg, double predkosc_lewa, double predkosc_prawa, double czas){
	QRectF wynik;
	
	if(predkosc_lewa != 0 || predkosc_prawa != 0){
		QVector2D kierunek(cos(czolg.kierunek), -sin(czolg.kierunek));
		QPointF przemieszczenie;
		double kat = 0.0;
		
		if(predkosc_lewa == predkosc_prawa){
			przemieszczenie = (kierunek * predkosc_prawa * czolg.predkosc_maksymalna * czas).toPointF();
		}else if(predkosc_lewa == -predkosc_prawa){
			kat = 2 * predkosc_prawa * czolg.predkosc_maksymalna * czas / this->czolg.rozmiar.width();
			wynik = QRectF(czolg.pozycja - QPointF(1, 1), czolg.pozycja + QPointF(1, 1));
		}else{
			double droga_lewa, droga_prawa, promien_skretu;
			
			droga_lewa = predkosc_lewa * czolg.predkosc_maksymalna * czas;
			droga_prawa = predkosc_prawa * czolg.predkosc_maksymalna * czas;
			promien_skretu = (2 * czolg.rozmiar.width() * droga_prawa / (droga_prawa - droga_lewa) - czolg.rozmiar.width()) / 2;
			kat = (droga_lewa + droga_prawa) / (2 * promien_skretu);
		
			QVector2D przemieszczenie_wzgledne(promien_skretu * cos(kat) - promien_skretu, promien_skretu * sin(kat));
			QVector2D przemieszczenie_bezwzgledne_dla_wzglednego_x(przemieszczenie_wzgledne.x() * -kierunek.y(), przemieszczenie_wzgledne.x() * kierunek.x());
			QVector2D przemieszczenie_bezwzgledne_dla_wzglednego_y(przemieszczenie_wzgledne.y() * kierunek.x(), przemieszczenie_wzgledne.y() * kierunek.y());
			QVector2D przemieszczenie_bezwzgledne = przemieszczenie_bezwzgledne_dla_wzglednego_x + przemieszczenie_bezwzgledne_dla_wzglednego_y;
			
			przemieszczenie = przemieszczenie_bezwzgledne.toPointF();
			wynik = QRectF(czolg.pozycja + QPointF(promien_skretu * cos(czolg.kierunek + M_PI_2) - fabs(promien_skretu), promien_skretu * -sin(czolg.kierunek + M_PI_2) - fabs(promien_skretu)), QSizeF(2 * fabs(promien_skretu), 2 * fabs(promien_skretu)));
		}
		czolg.kierunek = fmod(czolg.kierunek + kat, 2 * M_PI);
	/*	if(!przemieszczenie.isNull()){
			double a, b, d2;
			int i = 0;
			
			do{
				a = (this->odcinki[i].y1() - this->odcinki->y2()) / (this->odcinki[i].x1() - this->odcinki->x2());
				b = (this->odcinki[i].x1() * this->odcinki->y2() - this->odcinki[i].y1() * this->odcinki->x2()) / (this->odcinki[i].x1() - this->odcinki->x2());
				d2 = pow(a * (czolg.pozycja.x() + przemieszczenie.x()) - czolg.pozycja.y() - przemieszczenie.y() + b, 2) / (a * a + 1);
			}while(2 * d2 > pow(this->czolg.rozmiar.width(), 2) + pow(this->czolg.rozmiar.height(), 2) && ++i < this->ilosc_odcinkow);
			qDebug() << d2;
			if(i == this->ilosc_odcinkow)*/
				czolg.pozycja += przemieszczenie;
	//	}
	}
	
	if(this->rotacja_wiezy != 0)
		this->czolg.kierunek_wiezy = fmod(this->czolg.kierunek_wiezy + this->rotacja_wiezy * czas * this->czolg.predkosc_obrotu_wieza, 2 * M_PI);
		
	return wynik;
}

void Plansza::rysuj_mape(QPainter& painter, QRect rect){
	double margines = sqrt(pow(this->czolg.rozmiar.width(), 2) + pow(this->czolg.rozmiar.height(), 2));
	
	if(this->czolg.pozycja.x() < this->widok.x() + margines)
		this->widok.setX(this->czolg.pozycja.x() - margines);
	else if(this->czolg.pozycja.x() > this->widok.x() + rect.width() - margines)
		this->widok.setX(this->czolg.pozycja.x() - rect.width() + margines);
		
	if(this->czolg.pozycja.y() < this->widok.y() + margines)
		this->widok.setY(this->czolg.pozycja.y() - margines);
	else if(this->czolg.pozycja.y() > this->widok.y() + rect.height() - margines)
		this->widok.setY(this->czolg.pozycja.y() - rect.height() + margines);
		
	int start;
	painter.setPen(0x004040);
	if(this->widok.x() > 0)
		start = this->widok.x() % 50 > 0 ? 50 - this->widok.x() % 50 : 0;
	else
		start = -this->widok.x() % 50;
	for(int i = 0; start + i * 50 < rect.width(); i++)
		painter.drawLine(rect.topLeft() + QPoint(start + i * 50, 0), rect.topLeft() + QPoint(start + i * 50, rect.height() - 1));
	if(this->widok.y() > 0)
		start = this->widok.y() % 50 > 0 ? 50 - this->widok.y() % 50 : 0;
	else
		start = -this->widok.y() % 50;
	for(int i = 0; start + i * 50 < rect.height(); i++)
		painter.drawLine(rect.topLeft() + QPoint(0, start + i * 50), rect.topLeft() + QPoint(rect.width() - 1, start + i * 50));
		
	painter.setPen(0xFF0000);
	for(int i = 0; i < this->ilosc_odcinkow; i++)
		painter.drawLine(this->odcinki[i].translated(rect.topLeft() - this->widok));
		
	QTransform t;
	t.rotateRadians(M_PI_2 - this->czolg.kierunek);
	QPixmap korpus = this->czolg.korpus.transformed(t);
	painter.drawPixmap(rect.topLeft() + this->czolg.pozycja - this->widok - QPoint(korpus.width() / 2, korpus.height() / 2), korpus);
	t.rotateRadians(-this->czolg.kierunek_wiezy);
	QPixmap wieza = this->czolg.wieza.transformed(t);
	QPointF przesuniecie_osi_wiezy_dla_korpusu(this->czolg.przesuniecie_osi_wiezy_dla_korpusu * -cos(this->czolg.kierunek), this->czolg.przesuniecie_osi_wiezy_dla_korpusu * sin(this->czolg.kierunek));
	QPointF przesuniecie_osi_wiezy_dla_wiezy(this->czolg.przesuniecie_osi_wiezy_dla_wiezy * cos(this->czolg.kierunek + this->czolg.kierunek_wiezy), this->czolg.przesuniecie_osi_wiezy_dla_wiezy * -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy));
	painter.drawPixmap(rect.topLeft() - this->widok + this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_osi_wiezy_dla_wiezy - QPoint(wieza.width() / 2, wieza.height() / 2), wieza);
	
	if(this->strzal){
		this->pociski << Pocisk(this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_osi_wiezy_dla_wiezy + QPointF(this->czolg.wieza.height() * cos(this->czolg.kierunek + this->czolg.kierunek_wiezy) / 2, this->czolg.wieza.height() * -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy) / 2), QVector2D(cos(this->czolg.kierunek + this->czolg.kierunek_wiezy), -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy)), 500);
		this->strzal = false;
	}
	painter.setBrush(QColor(Qt::white));
	for(int i = 0; i < this->pociski.size(); i++){
		this->pociski[i].pozycja += this->pociski[i].kierunek.toPointF() * this->pociski[i].predkosc * 0.02;
		painter.drawEllipse(rect.topLeft() - this->widok + this->pociski[i].pozycja.toPoint(), 3, 3);
	}
	
	if(this->o.width() > 0){
		painter.setPen(0x000080);
		painter.setBrush(QBrush(QColor(0, 0, 255, 32)));
		this->o.moveTopLeft(this->o.topLeft() + rect.topLeft() - this->widok);
		painter.drawEllipse(this->o);
	}
}

void Plansza::rysuj_drazek(QPainter& painter, QRect rect, double wychylenie){
	painter.fillRect(rect, 0xA0A0A0);
	painter.fillRect(QRect(rect.x() + 10, rect.y() + 10, rect.width() - 20, 20), 0xD0D0D0);
	painter.setPen(0x000080);
	painter.drawText(QRect(rect.x() + 10, rect.y() + 10, rect.width() - 20, 20), Qt::AlignCenter, QString::number(wychylenie));
	painter.fillRect(QRect(rect.x() + 0.45 * rect.width(), rect.y() + 40, 0.1 * rect.width(), rect.height() - 50), 0x101010);
	painter.fillRect(QRect(rect.x() + 0.1 * rect.width(), rect.y() + 13 + 0.5 * rect.height(), 0.8 * rect.width(), 4), 0x101010);
	painter.fillRect(QRect(rect.x() + 0.2 * rect.width(), rect.y() + 40 + (1 - wychylenie) * (rect.height() - 70) / 2, 0.6 * rect.width(), 20), 0xC00000);
}

void Plansza::paintEvent(QPaintEvent* e){
	QPainter painter(this);
	painter.fillRect(this->rect(), Qt::black);
	this->rysuj_mape(painter, QRect(0.1 * this->width(), 0, 0.8 * this->width(), this->height()));
	this->rysuj_drazek(painter, QRect(0, 0, 0.1 * this->width(), this->height()), 0.1 * this->predkosc[0]);
	this->rysuj_drazek(painter, QRect(0.9 * this->width(), 0, 0.1 * this->width(), this->height()), 0.1 * this->predkosc[1]);
	painter.end();
}

void Plansza::ruch(){
	this->o = this->przemiesc(this->czolg, this->predkosc[0], this->predkosc[1], 0.02);
	this->repaint();
}
