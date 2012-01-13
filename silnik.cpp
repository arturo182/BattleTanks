#include "silnik.h"
#include "klawiatura.h"
#include "bazadanych.h"
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
ramki(0),
milisekundy(0){
	this->bazaDanych = new BazaDanych;
	this->bazaDanych->polacz();

	if(this->bazaDanych->ustawienie("sterowanie", "gamepad").toString() == "gamepad")
		this->urzadzenieWejscia = new Gamepad;
	else
		this->urzadzenieWejscia = new Klawiatura;

	QString rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "1280x720").toString();
	QString jakosc = this->bazaDanych->ustawienie("jakosc", "niska").toString();
	this->ekran = new Ekran(qStringToSize(rozdzielczosc), jakosc);
//	this->ekran = new Ekran(QSize(1024, 768), jakosc);

	this->plansza = new Plansza(this->ekran);

	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(WYSOKOSC_WIDOKU);
	Tekstura::przeskalujWszystko(Obiekt::skala);

	this->menu = new Menu(this->ekran, this->bazaDanych, this->plansza);
	this->pauza = new Pauza(this->ekran, this->bazaDanych, this->plansza);
	this->logika = new Logika(this->plansza);

	this->urzadzenieWejscia->otworz();
	Dzwiek::glosnosc = this->bazaDanych->ustawienie("glosnosc", 5).toInt();

	this->zaladujSpecyfikacjeObiektow();
}

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
/*
	QSqlQuery pociski("SELECT * FROM pociski");
	while(pociski.next()) {
		this->plansza->dodajSpecyfikacje(
			new SpecyfikacjaPocisku(
				QPixmap("grafika/pociski/" + pociski.value(1).toString() + ".png"),
				pociski.value(2).toInt(), // zasieg
				pociski.value(3).toInt(), // predkosc
				pociski.value(4).toInt(), // silaRazenie
				pociski.value(5).toInt()  // promienRazenie
			)
		);
	}
*/

	QString nazwaBroni;
	for(int i = 1; i <= 2; i++){
		nazwaBroni = "grafika/bronie/bron" + QString::number(i);

		SpecyfikacjaAnimacji* animacja = new SpecyfikacjaAnimacji(
			QPixmap(nazwaBroni + "Eksplozja.png"),	//	tekstury
			QSize(4, 4),	//	iloscKlatek
			1000	//	czasTrwaniaMilisekundy
		);

		this->plansza->dodajSpecyfikacje(animacja);

		this->plansza->dodajSpecyfikacje(
			new SpecyfikacjaPocisku(
				QPixmap(nazwaBroni + "Pocisk.png"),
				i * 500,
				i * 500,
				i * 100,
				i * 100,
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

	//	sprawdzic czy koniec gry

	if(this->tryb == ROZGRYWKA)
		this->plansza->rysuj();
}

void Silnik::odswiezPauze(int milisekundy)
{
	this->tryb = this->pauza->odswiez(milisekundy, aktualnaAkcja());

	if(this->tryb == PAUZA)
		this->pauza->rysuj();
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

void Silnik::uruchom(){
	this->tryb = MENU;
	this->czasOstatniegoOdswiezenia = QTime::currentTime();
	this->ekran->show();
}

bool Silnik::czyWyjsc() const
{
	return this->tryb == WYJSCIE;
}
