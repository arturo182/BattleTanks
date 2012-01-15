#include <QFile>
#include <QDataStream>
#include <QDebug>
#include "plansza.h"
#include "ekran.h"
#include "pojazdgracza.h"
#include "pojazdobcy.h"
#include "pocisk.h"
#include "widzety.h"

Plansza::Plansza(Ekran* ekran):
	ekran(ekran),
	idPlanszy(-1),
	mapa(0),
	pojazdGracza(0),
	celownik(QPixmap("grafika/celownik.png")),
	status(0){}

Plansza::~Plansza(){
	this->czysc();
	qDeleteAll(this->specyfikacjePojazdow);
	qDeleteAll(this->specyfikacjePociskow);
	qDeleteAll(this->specyfikacjeAnimacji);
}

void Plansza::dodajSpecyfikacje(SpecyfikacjaPojazdu* specyfikacja){
	this->specyfikacjePojazdow.append(specyfikacja);
}

void Plansza::dodajSpecyfikacje(SpecyfikacjaPocisku* specyfikacja){
	this->specyfikacjePociskow.append(specyfikacja);
}

void Plansza::dodajSpecyfikacje(SpecyfikacjaAnimacji* specyfikacja){
	this->specyfikacjeAnimacji.append(specyfikacja);
}

bool Plansza::zaladuj(int id, QString nazwaPlanszy){
	QPixmap mapaTekstura("plansze/" + nazwaPlanszy + ".png");

	if(mapaTekstura.isNull())
		return false;
	this->mapa = new Tekstura(mapaTekstura);
	this->widok = QPoint(0, 0);
	
	QFile mapaSpecyfikacjaPlik("plansze/" + nazwaPlanszy + ".dat");
	if(!mapaSpecyfikacjaPlik.open(QIODevice::ReadOnly)){
		delete this->mapa;
		return false;
	}
	
	this->idPlanszy = id;
	
	QDataStream mapaSpecyfikacjaDane(&mapaSpecyfikacjaPlik);
	int iloscElementow, n;
	QPoint punkt;

	mapaSpecyfikacjaDane >> n;
	this->trybGry = static_cast<TrybGry>(n);
	if(this->trybGry == LABIRYNT){
		mapaSpecyfikacjaDane >> punkt;
		this->wyjscie = punkt;
	}
	
	mapaSpecyfikacjaDane >> iloscElementow;
	for(int i = 0; i < iloscElementow; i++){
		mapaSpecyfikacjaDane >> n;
		QPolygonF przeszkoda;
		for(int j = 0; j < n; j++){
			mapaSpecyfikacjaDane >> punkt;
			przeszkoda << punkt;
		}
		this->przeszkody.append(przeszkoda);
	}
	
	mapaSpecyfikacjaDane >> iloscElementow;
	this->graf.utworz(iloscElementow);
	for(int i = 0; i < iloscElementow; i++){
		mapaSpecyfikacjaDane >> punkt;
		this->graf.ustawWierzcholek(i, punkt);
	}
	
	mapaSpecyfikacjaDane >> iloscElementow;
	int v, w;
	for(int i = 0; i < iloscElementow; i++){
		mapaSpecyfikacjaDane >> v >> w;
		this->graf.dodajKrawedz(v, w);
	}
	
	int nrSpecyfikacjiPojazdu, nrSpecyfikacjiPociskow;
	float zwrot;
	
	mapaSpecyfikacjaDane >> nrSpecyfikacjiPojazdu >> punkt >> zwrot;
	this->pojazdGracza = new PojazdGracza(
		this->specyfikacjePojazdow[nrSpecyfikacjiPojazdu],
		punkt,
		zwrot,
		this->specyfikacjePociskow.size()
	);
	
	mapaSpecyfikacjaDane >> iloscElementow;
	for(int i = 0; i < iloscElementow; i++){
		mapaSpecyfikacjaDane >> nrSpecyfikacjiPociskow >> n;
		this->pojazdGracza->dodajPociski(nrSpecyfikacjiPociskow, n);
	}
	
	mapaSpecyfikacjaDane >> iloscElementow;
	for(int i = 0; i < iloscElementow; i++){
		mapaSpecyfikacjaDane >> nrSpecyfikacjiPojazdu >> v >> zwrot >> nrSpecyfikacjiPociskow;
		this->pojazdyObce.append(
			new PojazdObcy(
				this->specyfikacjePojazdow[nrSpecyfikacjiPojazdu],
				this->graf.pozycjaWierzcholka(v),
				v,
				zwrot,
				nrSpecyfikacjiPociskow
			)
		);
	}
	
	this->status = 1;
	
	mapaSpecyfikacjaPlik.close();
	return true;
}

void Plansza::czysc(){
	delete this->mapa;
	this->mapa = 0;

	delete this->pojazdGracza;
	this->pojazdGracza = 0;

	this->przeszkody.clear();
	this->graf.czysc();

	qDeleteAll(this->pojazdyObce);
	this->pojazdyObce.clear();

	qDeleteAll(this->pociski);
	this->pociski.clear();

	qDeleteAll(this->animacje);
	this->animacje.clear();

	qDeleteAll(this->bonusy);
	this->bonusy.clear();
	
	this->idPlanszy = -1;
	this->status = 0;
}

void Plansza::rysuj(){
	if(this->status == 0)
		return;
	
	QPainter painter(&this->ekran->buforObrazu);
	
	this->odswiezWidok();
	this->rysujMape(painter);
	
	
	
	//	DEBUG BEGIN
	//	usunac przyjazn z klas: Graf, Wierzcholek, PojazdObcy
	
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(255, 0, 0, 128));
	for(QList<QPolygonF>::iterator i = this->przeszkody.begin(); i != this->przeszkody.end(); i++)
		painter.drawPolygon(i->translated(-widok));
	
	painter.setPen(Qt::blue);
	for(int i = 0; i < this->graf.iloscWierzcholkow; i++)
		for(QList<int>::iterator j = this->graf.wierzcholki[i]->krawedzie.begin(); j != this->graf.wierzcholki[i]->krawedzie.end(); j++)
			painter.drawLine(this->graf.wierzcholki[i]->pozycja - widok, this->graf.pozycjaWierzcholka(*j) - widok);
	
	painter.setPen(Qt::red);
	for(QList<PojazdObcy*>::iterator i = this->pojazdyObce.begin(); i != this->pojazdyObce.end(); i++){
		int v = (*i)->v;
		while(v != this->graf.nastepnyWierzcholek(v)){
			painter.drawLine(this->graf.pozycjaWierzcholka(v) - widok, this->graf.pozycjaWierzcholka(this->graf.nastepnyWierzcholek(v)) - widok);
			v = this->graf.nastepnyWierzcholek(v);
		}
	}
	
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::blue);
	for(int i = 0; i < this->graf.iloscWierzcholkow; i++)
		painter.drawEllipse(this->graf.pozycjaWierzcholka(i) - widok, 5, 5);
		
	//	DEBUG END
		
		
	
	
	
	for(QList<PojazdObcy*>::iterator i = this->pojazdyObce.begin(); i != this->pojazdyObce.end(); i++)
		(*i)->rysuj(painter, this->widok);
	this->pojazdGracza->rysuj(painter, this->widok);
	
	this->rysujAnimacje(painter);
	this->rysujPociski(painter);
	this->rysujCelownik(painter);
	
	painter.drawPixmap(
		Obiekt::skala * QPointF(20.0, WYSOKOSC_WIDOKU - this->specyfikacjePociskow[this->pojazdGracza->aktualnaBron]->tekstura.teksturaOryginalna.height() - 20),
		this->specyfikacjePociskow[this->pojazdGracza->aktualnaBron]->tekstura.teksturaPrzeskalowana
	);
	painter.setFont(QFont("Trebuchet MS", Obiekt::skala * 20, QFont::Bold));
	painter.setPen(Qt::white);
	Widzety::cieniowanyTekst(painter, QRectF(
		Obiekt::skala * QPointF(20.0 + this->specyfikacjePociskow[this->pojazdGracza->aktualnaBron]->tekstura.teksturaOryginalna.width(), WYSOKOSC_WIDOKU - 50),
		QSize(100, 50)
	), " x " + (this->pojazdGracza->zapasPociskow() < 0 ? "INF" : QString::number(this->pojazdGracza->zapasPociskow())));
/*	painter.drawText(
		Obiekt::skala * QPointF(20.0 + this->specyfikacjePociskow[this->pojazdGracza->aktualnaBron]->tekstura.teksturaOryginalna.width(), WYSOKOSC_WIDOKU - 23),
		" x " + (this->pojazdGracza->zapasPociskow() < 0 ? "INF" : QString::number(this->pojazdGracza->zapasPociskow()))
	);*/
	
	painter.end();
	this->ekran->update();
}

void Plansza::rysujMape(QPainter& painter){
	painter.drawPixmap(
		QPoint(0, 0),
		this->mapa->teksturaPrzeskalowana,
		QRect(this->widok, this->ekran->buforObrazu.size())
	);
}

void Plansza::rysujAnimacje(QPainter& painter){
	for(QList<Animacja*>::iterator i = this->animacje.begin(); i != this->animacje.end(); i++)
		(*i)->rysuj(painter, this->widok);
}

void Plansza::rysujPociski(QPainter& painter){
	for(QList<Pocisk*>::iterator i = this->pociski.begin(); i != this->pociski.end(); i++)
		(*i)->rysuj(painter, this->widok);
}

void Plansza::rysujCelownik(QPainter& painter){
	painter.drawPixmap(
		Obiekt::skala * (this->pojazdGracza->punktWylotuLufy() + this->pojazdGracza->celownikOdleglosc * this->pojazdGracza->zwrotWiezyWektor.toPointF()) - this->widok - QPointF(this->celownik.teksturaPrzeskalowana.width() / 2.0, this->celownik.teksturaPrzeskalowana.height() / 2.0),
		this->celownik.teksturaPrzeskalowana
	);
}

void Plansza::odswiezWidok(){
	int margines = Obiekt::skala * MARGINES;

	if(Obiekt::skala * this->pojazdGracza->pozycja.x() - this->widok.x() < margines){
		if(Obiekt::skala * this->pojazdGracza->pozycja.x() > margines)
			this->widok.setX(Obiekt::skala * this->pojazdGracza->pozycja.x() - margines);
		else
			this->widok.setX(0);
	}else if(this->widok.x() + this->ekran->buforObrazu.width() - Obiekt::skala * this->pojazdGracza->pozycja.x() < margines){
		if(this->mapa->teksturaPrzeskalowana.width() - Obiekt::skala * this->pojazdGracza->pozycja.x() > margines)
			this->widok.setX(Obiekt::skala * this->pojazdGracza->pozycja.x() + margines - this->ekran->buforObrazu.width());
		else
			this->widok.setX(this->mapa->teksturaPrzeskalowana.width() - this->ekran->buforObrazu.width());
	}

	if(Obiekt::skala * this->pojazdGracza->pozycja.y() - this->widok.y() < margines){
		if(Obiekt::skala * this->pojazdGracza->pozycja.y() > margines)
			this->widok.setY(Obiekt::skala * this->pojazdGracza->pozycja.y() - margines);
		else
			this->widok.setY(0);
	}else if(this->widok.y() + this->ekran->buforObrazu.height() - Obiekt::skala * this->pojazdGracza->pozycja.y() < margines){
		if(this->mapa->teksturaPrzeskalowana.height() - Obiekt::skala * this->pojazdGracza->pozycja.y() > margines)
			this->widok.setY(Obiekt::skala * this->pojazdGracza->pozycja.y() + margines - this->ekran->buforObrazu.height());
		else
			this->widok.setY(this->mapa->teksturaPrzeskalowana.height() - this->ekran->buforObrazu.height());
	}
}
