#include <cmath>
#include <QVector2D>
#include <QDebug>
#include <QMessageBox>
#include "plansza.h"

Plansza::Plansza(QPixmap& mapa, QWidget* parent):
	QLabel(parent),
	mapa(mapa),
	widok(QPoint(0, 0)),
	czolg(QPointF(100, 100), QPixmap(":/tank_korpus.png"), QPixmap(":/tank_wieza.png"), -10, 23, 0, 100, M_PI_2),
	rotacja_wiezy(0),
	predkosc_lufy(300),
	podnoszenie_lufy(0),
	cel_odleglosc(100),
	strzal(false),
	o(0, 0, 0, 0){
	connect(&this->timer, SIGNAL(timeout()), this, SLOT(ruch()));
	SDL_Init(SDL_INIT_JOYSTICK);
	this->joystick = SDL_JoystickOpen(0);
	this->resize(800, 600);
	
	this->tekstury = reinterpret_cast<Tekstury*>(malloc(5 * sizeof(Tekstury)));
	new(this->tekstury) Tekstury(":/eksplozja2.png", QSize(4, 4));
	new(this->tekstury + 1) Tekstury(":/ring.png", QSize(4, 4));
	
	this->ilosc_obiektow = 0;//1;
	//this->obiekty = new QPolygonF[this->ilosc_obiektow];
	//this->obiekty[0] << QPointF(100, 200) << QPointF(200, 200) << QPointF(200, 500) << QPointF(100, 500);
	
	//new(this->dzwieki) QSound("wystrzal2.wav");
	//new(this->dzwieki + 1) QSound("wybuch.wav");
	//new(this->dzwieki + 2) QSound("wieza.wav");
	
	this->predkosc[0] = this->predkosc[1] = 0;
	this->timer.start(20);
}

Plansza::~Plansza(){
	delete[] this->obiekty;
	free(this->tekstury);
	SDL_JoystickClose(this->joystick);
	SDL_Quit();
}

double Plansza::zwrot(QPointF a, QPointF b, QPointF c){
	return a.x() * b.y() + b.x() * c.y() + c.x() * a.y() - b.y() * c.x() - c.y() * a.x() - a.y() * b.x();
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
		
		QPointF pozycja_docelowa = czolg.pozycja + przemieszczenie;
		double kierunek_docelowy = fmod(czolg.kierunek + kat, 2 * M_PI);
		
		QVector2D v1(this->czolg.rozmiar.height() * cos(kierunek_docelowy) / 2, this->czolg.rozmiar.height() * -sin(kierunek_docelowy) / 2);
		QVector2D v2(v1.y() * this->czolg.rozmiar.width() / this->czolg.rozmiar.height(), -v1.x() * this->czolg.rozmiar.width() / this->czolg.rozmiar.height());
		
		QPointF p[4] = {
			QPointF(v1.x() + v2.x(), v1.y() + v2.y()) + pozycja_docelowa,
			QPointF(v1.x() - v2.x(), v1.y() - v2.y()) + pozycja_docelowa,
			QPointF(-v1.x() - v2.x(), -v1.y() - v2.y()) + pozycja_docelowa,
			QPointF(-v1.x() + v2.x(), -v1.y() + v2.y()) + pozycja_docelowa
		};
		QPoint m[4] = {
			this->mapa.rect().topLeft(),
			this->mapa.rect().topRight(),
			this->mapa.rect().bottomRight(),
			this->mapa.rect().bottomLeft()
		};
		
		int i = 0, j, k;
		while(i < 4){
			j = 1;
			while(j < 4 && this->zwrot(m[j - 1], m[j], p[i]) >= 0)
				j++;
			if(j < 4 || this->zwrot(m[3], m[0], p[i]) < 0)
				break;
			i++;
		}
		if(i == 4){
			i = 0;
			while(i < this->ilosc_obiektow){
				j = 0;
				while(j < 4){
					k = 1;
					while(k < this->obiekty[i].size() && this->zwrot(this->obiekty[i][k - 1], this->obiekty[i][k], p[j]) > 0)
						k++;
					if(k == this->obiekty[i].size() && this->zwrot(this->obiekty[i][this->obiekty[i].size() - 1], this->obiekty[i][0], p[j]) > 0)
						break;
					j++;
				}
				if(j < 4)
					break;
				j = 0;
				while(j < this->obiekty[i].size()){
					k = 1;
					while(k < 4 && this->zwrot(p[k - 1], p[k], this->obiekty[i][j]) > 0)
						k++;
					if(k == 4 && this->zwrot(p[3], p[0], this->obiekty[i][j]) > 0)
						break;
					j++;
				}
				if(j < this->obiekty[i].size())
					break;
				i++;
			}
			if(i == this->ilosc_obiektow){
				this->czolg.pozycja = pozycja_docelowa;
				this->czolg.kierunek = kierunek_docelowy;
			}
		}
	}
	
	if(this->rotacja_wiezy != 0)
		this->czolg.kierunek_wiezy = fmod(this->czolg.kierunek_wiezy + this->rotacja_wiezy * czas * this->czolg.predkosc_obrotu_wieza, 2 * M_PI);
	
	if(this->podnoszenie_lufy != 0){
		this->cel_odleglosc += 0.02 * this->podnoszenie_lufy * this->predkosc_lufy;
		if(this->cel_odleglosc > 500)
			this->cel_odleglosc = 500;
		else if(this->cel_odleglosc < 100)
			this->cel_odleglosc = 100;
	}
	
	return wynik;
}

void Plansza::rysuj_mape(QPainter& painter, QRect rect){
	painter.setPen(0xFF0000);
	for(int i = 0; i < this->ilosc_obiektow; i++)
		painter.drawPolygon(this->obiekty[i].translated(rect.topLeft() - this->widok));

	double margines = 2 * sqrt(pow(this->czolg.rozmiar.width(), 2) + pow(this->czolg.rozmiar.height(), 2));
	
	if(this->czolg.pozycja.x() < this->widok.x() + margines)
		this->widok.setX(this->czolg.pozycja.x() > margines ? this->czolg.pozycja.x() - margines : 0);
	else if(this->czolg.pozycja.x() > this->widok.x() + rect.width() - margines)
		this->widok.setX(this->czolg.pozycja.x() + margines < this->mapa.width() ? this->czolg.pozycja.x() - rect.width() + margines : this->mapa.width() - rect.width());
		
	if(this->czolg.pozycja.y() < this->widok.y() + margines)
		this->widok.setY(this->czolg.pozycja.y() > margines ? this->czolg.pozycja.y() - margines : 0);
	else if(this->czolg.pozycja.y() > this->widok.y() + rect.height() - margines)
		this->widok.setY(this->czolg.pozycja.y() + margines < this->mapa.height() ? this->czolg.pozycja.y() - rect.height() + margines : this->mapa.height() - rect.height());
		
	painter.drawPixmap(rect, this->mapa, QRect(this->widok, rect.size()));
	
	QTransform t;
	t.rotateRadians(M_PI_2 - this->czolg.kierunek);
	QPixmap korpus = this->czolg.korpus.transformed(t, Qt::SmoothTransformation);
	painter.drawPixmap(rect.topLeft() + this->czolg.pozycja - this->widok - QPoint(korpus.width() / 2, korpus.height() / 2), korpus);
	t.rotateRadians(-this->czolg.kierunek_wiezy);
	QPixmap wieza = this->czolg.wieza.transformed(t, Qt::SmoothTransformation);
	QPointF przesuniecie_osi_wiezy_dla_korpusu(this->czolg.przesuniecie_osi_wiezy_dla_korpusu * -cos(this->czolg.kierunek), this->czolg.przesuniecie_osi_wiezy_dla_korpusu * sin(this->czolg.kierunek));
	QPointF przesuniecie_osi_wiezy_dla_wiezy(this->czolg.przesuniecie_osi_wiezy_dla_wiezy * cos(this->czolg.kierunek + this->czolg.kierunek_wiezy), this->czolg.przesuniecie_osi_wiezy_dla_wiezy * -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy));
	painter.drawPixmap(rect.topLeft() - this->widok + this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_osi_wiezy_dla_wiezy - QPoint(wieza.width() / 2, wieza.height() / 2), wieza);
	
	QPointF przesuniecie_wylotu_lufy_dla_osi_wiezy(
		(this->czolg.przesuniecie_osi_wiezy_dla_wiezy + this->czolg.wieza.height() / 2 + 3) * cos(this->czolg.kierunek + this->czolg.kierunek_wiezy),
		(this->czolg.przesuniecie_osi_wiezy_dla_wiezy + this->czolg.wieza.height() / 2 + 3) * -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy)
	);
	QPointF przesuniecie_celu_dla_lufy(
		this->cel_odleglosc * cos(this->czolg.kierunek + this->czolg.kierunek_wiezy),
		this->cel_odleglosc * -sin(this->czolg.kierunek + this->czolg.kierunek_wiezy)
	);
	
	QTime time = QTime::currentTime();
	if(this->strzal){
		this->pociski << Pocisk(
			this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_wylotu_lufy_dla_osi_wiezy,
			this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_wylotu_lufy_dla_osi_wiezy + przesuniecie_celu_dla_lufy,
			700,
			time);
		this->strzal = false;
		this->dzwieki << new QSound("wystrzal2.wav");
		this->dzwieki.last()->play();
	}
	
	for(int i = 0; i < this->bonusy.size(); i++)
		if(sqrt(pow(this->bonusy[i].pozycja.x() - this->czolg.pozycja.x(), 2) + pow(this->bonusy[i].pozycja.y() - this->czolg.pozycja.y(), 2)) < 70)
			this->bonusy[i].zakoncz();
	
	for(int i = this->eksplozje.size() - 1; i >= 0; i--)
		if(!this->eksplozje[i].rysuj(painter, this->widok, rect, time)){
			this->bonusy << Animacja(this->eksplozje[i].pozycja, this->tekstury[1], time, 1000, true);
			this->eksplozje.removeAt(i);
		}
	
	for(int i = this->bonusy.size() - 1; i >= 0; i--)
		if(!this->bonusy[i].rysuj(painter, this->widok, rect, time))
			this->bonusy.removeAt(i);
	
	painter.setBrush(QColor(Qt::white));
	for(int i = this->pociski.size() - 1; i >= 0; i--)
		if(this->pociski[i].przesun(time)){
			this->eksplozje.prepend(Animacja(this->pociski[i].pozycja().toPoint(), this->tekstury[0], time, 1000));
			this->pociski.removeAt(i);
			this->dzwieki << new QSound("wybuch.wav");
			this->dzwieki.last()->play();
		}else
			painter.drawEllipse(this->pociski[i].pozycja() - this->widok + rect.topLeft(), 3, 3);
			
	painter.drawPixmap(rect.topLeft() - this->widok + this->czolg.pozycja + przesuniecie_osi_wiezy_dla_korpusu + przesuniecie_wylotu_lufy_dla_osi_wiezy + przesuniecie_celu_dla_lufy - QPoint(13, 13), QPixmap(":/target3.png"));
	
	if(this->o.width() > 0){
		painter.setPen(0x000080);
		painter.setBrush(QBrush(QColor(0, 0, 255, 32)));
		this->o.moveTopLeft(this->o.topLeft() + rect.topLeft() - this->widok);
		painter.drawEllipse(this->o);
	}
}

void Plansza::rysuj_drazek(QPainter& painter, QRect rect, double wychylenie){
	painter.fillRect(rect, Qt::black);
	painter.fillRect(QRect(rect.x() + (rect.width() - 4) / 2, rect.y() + 10, 4, rect.height() - 20), 0x202020);
	painter.fillRect(QRect(rect.x() + (rect.width() - 40) / 2, rect.y() + 10 + (1 - wychylenie) * (rect.height() - 40) / 2, 40, 20), 0x808080);
}

void Plansza::paintEvent(QPaintEvent* e){
	QPainter painter(this);
	this->rysuj_mape(painter, QRect(50, 0, this->width() - 100, this->height()));
	this->rysuj_drazek(painter, QRect(0, 0, 50, this->height()), this->predkosc[0]);
	this->rysuj_drazek(painter, QRect(this->width() - 50, 0, 50, this->height()), this->predkosc[1]);
	painter.end();
}

void Plansza::ruch(){
	SDL_JoystickUpdate();
	this->predkosc[0] = -double(SDL_JoystickGetAxis(this->joystick, 1)) / (1 << 15);
	this->predkosc[1] = -double(SDL_JoystickGetAxis(this->joystick, 3)) / (1 << 15);
	
	this->rotacja_wiezy = 0;
	if(SDL_JoystickGetHat(this->joystick, 0) & SDL_HAT_LEFT || SDL_JoystickGetButton(this->joystick, 6))
		this->rotacja_wiezy++;
	if(SDL_JoystickGetHat(this->joystick, 0) & SDL_HAT_RIGHT || SDL_JoystickGetButton(this->joystick, 7))
		this->rotacja_wiezy--;
		
	this->podnoszenie_lufy = 0;
	if(SDL_JoystickGetHat(this->joystick, 0) & SDL_HAT_UP)
		this->podnoszenie_lufy++;
	if(SDL_JoystickGetHat(this->joystick, 0) & SDL_HAT_DOWN)
		this->podnoszenie_lufy--;
		
	static bool wcisniety = false;
	if(!SDL_JoystickGetButton(this->joystick, 5))
		wcisniety = false;
	else if(!wcisniety){
		this->strzal = true;
		wcisniety = true;
	}
	
	if(SDL_JoystickGetButton(this->joystick, 9)){
		QMessageBox mb;
		mb.setText(QString::number(this->bonusy.size()));
		mb.exec();
	}
	
	this->o = this->przemiesc(this->czolg, this->predkosc[0], this->predkosc[1], 0.02);
	this->repaint();
}
