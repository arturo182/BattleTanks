#include "silnik.h"
#include "klawiatura.h"
#include "bazadanych.h"
#include "ladowanie.h"
#include "funkcje.h"
#include "plansza.h"
#include "gamepad.h"
#include "logika.h"
#include "dzwiek.h"
#include "ekran.h"
#include "pauza.h"
#include "menu.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

Silnik::Silnik():
	tryb(LADOWANIE),
	ramki(0),
	milisekundy(0),
	fps(0){}

Silnik::~Silnik(){
	delete this->logika;
	delete this->menu;
	delete this->pauza;
	delete this->plansza;
	delete this->ekran;
	delete this->bazaDanych;

	//to robi krzak na koncu programu, do dalszego zbadania
	//delete this->urzadzenieWejscia;
}

void Silnik::uruchom()
{
	this->czasOstatniegoOdswiezenia = QTime::currentTime();

	//baza danych
	this->bazaDanych = new BazaDanych;
	this->bazaDanych->polacz();

	//ekran
	QString rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "1280x720").toString();
	QString jakosc = this->bazaDanych->ustawienie("jakosc", "niska").toString();

	this->ekran = new Ekran(qStringToSize(rozdzielczosc), jakosc);
//	this->ekran = new Ekran(QSize(1024, 768), jakosc);
	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(WYSOKOSC_WIDOKU);

	//ladowanie
	this->ladowanie = new Ladowanie(this->ekran);
	this->ekran->show();
	this->odswiezLadowanie(0.0, "Ładowanie urządzenia wejścia");

	//urzadzenie wejscia
	if(this->bazaDanych->ustawienie("sterowanie", "gamepad").toString() == "gamepad")
		this->urzadzenieWejscia = new Gamepad;
	else
		this->urzadzenieWejscia = new Klawiatura;
	this->odswiezLadowanie(0.05, "Tworzenie planszy");

	//plansza
	this->plansza = new Plansza(this->ekran);
	this->odswiezLadowanie(0.1, "Skalowanie tekstur");

	//skalowanie tekstur
	Tekstura::przeskalujWszystko(Obiekt::skala);
	this->odswiezLadowanie(0.15, "Tworzenie menu");

	//menu
	this->menu = new Menu(this->ekran, this->bazaDanych, this->plansza, this->ladowanie);
	this->odswiezLadowanie(0.70, "Tworzenie ekranu pauzy");

	//pauza
	this->pauza = new Pauza(this->ekran, this->bazaDanych, this->plansza);
	this->odswiezLadowanie(0.75, "Tworzenie logiki");

	//logika
	this->logika = new Logika(this->plansza);
	this->odswiezLadowanie(0.70, "Inicjalizacja urządzenia wejścia");

	//inicjalizacja urzadzenia wejscia
	this->urzadzenieWejscia->otworz();
	this->odswiezLadowanie(0.85, "Ustawienie głośności");

	//glosnosc
	Dzwiek::glosnosc = this->bazaDanych->ustawienie("glosnosc", 5).toInt();
	this->odswiezLadowanie(0.90, "Ładowanie specyfikacji obiektów");

	//ladowanie specyfikacji
	this->zaladujSpecyfikacjeObiektow();
	this->odswiezLadowanie(1.0, "Ładowanie ukończone");

	//gotowe!!
	this->tryb = MENU;
}

void Silnik::zaladujSpecyfikacjeObiektow(){
	//	dodac specyfikacje obiektow do planszy

	QSqlQuery pojazdy("SELECT * FROM pojazdy");
	while(pojazdy.next()){
		QString nazwaPojazdu = pojazdy.value(1).toString();

		this->plansza->dodajSpecyfikacje(
			new SpecyfikacjaPojazdu(
				QPixmap("grafika/pojazdy/" + nazwaPojazdu + "Korpus.png"),
				QPixmap("grafika/pojazdy/" + nazwaPojazdu + "Wieza.png"),
				pojazdy.value(2).toFloat(),
				pojazdy.value(3).toFloat(),
				pojazdy.value(4).toFloat(),
				pojazdy.value(5).toFloat(),
				pojazdy.value(6).toInt()
			)
		);
	}

	QString nazwaBroni;
	QSqlQuery pociski("SELECT * FROM pociski");
	while(pociski.next()) {
		nazwaBroni = "grafika/bronie/" + pociski.value(1).toString();

		SpecyfikacjaAnimacji* animacja = new SpecyfikacjaAnimacji(
			QPixmap(nazwaBroni + "Eksplozja.png"),	//	tekstury
			QSize(pociski.value(6).toInt(), pociski.value(7).toInt()),	//	iloscKlatek
			pociski.value(8).toInt()	//	czasTrwaniaMilisekundy
		);

		this->plansza->dodajSpecyfikacje(animacja);

		this->plansza->dodajSpecyfikacje(
			new SpecyfikacjaPocisku(
				QPixmap(nazwaBroni + "Pocisk.png"),
				pociski.value(2).toInt(), // zasieg
				pociski.value(3).toInt(), // predkosc
				pociski.value(4).toInt(), // silaRazenie
				pociski.value(5).toInt(),  // promienRazenie
				animacja
			)
		);
	}
}

void Silnik::odswiezMenu(int milisekundy){
	this->tryb = this->menu->odswiez(milisekundy, aktualnaAkcja());

	if(this->tryb == MENU)
		this->menu->rysuj();
}

void Silnik::odswiezRozgrywke(int milisekundy){
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::PAUZA)) {
		this->plansza->rysuj();
		this->pauza->ustawTlo();
		this->tryb = PAUZA;
		return;
	}

	float predkoscGasienicyLewej = -this->urzadzenieWejscia->statusDzojstik(UrzadzenieWejscia::LEWY_PIONOWY);
	float predkoscGasienicyPrawej = -this->urzadzenieWejscia->statusDzojstik(UrzadzenieWejscia::PRAWY_PIONOWY);

	int rotacjaWiezy = 0;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::LEWO || this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WIEZA_LEWO))
		rotacjaWiezy++;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::PRAWO || this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WIEZA_PRAWO))
		rotacjaWiezy--;

	int zmianaBroni = 0;
	if(
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_PLUS) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::BRON_PLUS2)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_PLUS2) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::BRON_PLUS))
	)
		zmianaBroni++;
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_MINUS))
		zmianaBroni--;

	int zmianaZasiegu = 0;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::GORA)
		zmianaZasiegu++;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::DOL)
		zmianaZasiegu--;

	bool wystrzal = (
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYSTRZAL) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WYSTRZAL2)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYSTRZAL2) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WYSTRZAL))
	);

	this->logika->odswiez(milisekundy, predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaBroni, zmianaZasiegu, wystrzal);

//	if(this->plansza->koniecGry())
//		zakonczyc gre
//		wyniki wkrotce :P

	if(this->tryb == ROZGRYWKA)
		this->plansza->rysuj();
}

void Silnik::odswiezPauze(int milisekundy)
{
	this->tryb = this->pauza->odswiez(milisekundy, aktualnaAkcja());

	if(this->tryb == PAUZA)
		this->pauza->rysuj();
}

void Silnik::odswiezLadowanie(float postep, const QString &opis)
{
	this->ladowanie->odswiez(postep, opis);
}

Silnik::Akcja Silnik::aktualnaAkcja() const
{
	if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::GORA)
		return Silnik::GORA;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::DOL)
		return Silnik::DOL;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::PRAWO)
		return Silnik::PRAWO;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::LEWO)
		return Silnik::LEWO;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::USUN))
		return Silnik::USUN;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYBIERZ))
		return Silnik::WYBIERZ;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYBIERZ_ALT))
		return Silnik::WYBIERZ_ALT;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::COFNIJ))
		return Silnik::COFNIJ;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::PAUZA))
		return Silnik::START;

	return Silnik::BRAK;
}

void Silnik::odswiez(){
	QTime czasAktualny = QTime::currentTime();
	int milisekundy = this->czasOstatniegoOdswiezenia.msecsTo(czasAktualny);

	if(milisekundy == 0)
		return;

	this->urzadzenieWejscia->odswiez();
	switch(this->tryb){
		case MENU:
			this->odswiezMenu(milisekundy);
			break;
		case ROZGRYWKA:
			this->odswiezRozgrywke(milisekundy);
			break;
		case PAUZA:
			this->odswiezPauze(milisekundy);
			break;
		default:
			break;
	}

	this->ramki++;
	this->milisekundy += milisekundy;

	if(this->milisekundy >= 1000) {
		this->fps = this->ramki;
		this->ramki = 0;
		this->milisekundy = 0;
	}

	QPainter painter(&this->ekran->buforObrazu);
	painter.setFont(QFont("Consolas", this->ekran->buforObrazu.height() * 0.04));

	QPainterPath path;
	path.addText(10, painter.fontMetrics().ascent(), painter.font(), QString::number(this->fps));
	painter.strokePath(path, QPen(Qt::black, 2));
	painter.fillPath(path, QBrush(Qt::yellow));

	this->czasOstatniegoOdswiezenia = czasAktualny;
}

bool Silnik::czyWyjsc() const
{
	return this->tryb == WYJSCIE;
}
