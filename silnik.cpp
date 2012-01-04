#include "silnik.h"
#include "gamepad.h"
#include "klawiatura.h"
#include "ekran.h"
#include "bazadanych.h"
#include "plansza.h"
#include "menu.h"
#include "logika.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

Silnik::Silnik(){
	this->bazaDanych = new BazaDanych;
	this->bazaDanych->polacz();

	if(this->bazaDanych->ustawienie("sterowanie", "gamepad").toString() == "gamepad")
		this->urzadzenieWejscia = new Gamepad;
	else
		this->urzadzenieWejscia = new Klawiatura;

	this->ekran = new Ekran;
	QString rozdzielczosc = this->bazaDanych->ustawienie("rozdzielczosc", "800x600").toString();
	this->ekran->ustawRozdzielczosc(qStringToSize(rozdzielczosc));

	this->plansza = new Plansza(this->ekran, 1080, 400);

	this->menu = new Menu(this->ekran, this->bazaDanych, this->plansza);
	this->logika = new Logika(this->plansza);

	this->urzadzenieWejscia->otworz();

	this->menu->ladujMuzyke();

	this->ekran->ustawJakosc(this->bazaDanych->ustawienie("jakosc", "niska").toString());
	Obiekt::skala = float(this->ekran->buforObrazu.height()) / float(this->plansza->wysokoscWidoku());
	this->zaladujSpecyfikacjeObiektow();

	//	dopiac kontrole uruchomienia poszczegolnych elementow
}

Silnik::~Silnik(){
	delete this->logika;
	delete this->menu;
	delete this->plansza;
	delete this->ekran;
	delete this->bazaDanych;

	//to robi krzak na koncu programu, do dalszego zbadania
	//delete this->urzadzenieWejscia;
}

void Silnik::zaladujSpecyfikacjeObiektow(){
	//	dodac specyfikacje obiektow do planszy

	QSqlQuery pojazdy("SELECT * FROM pojazdy");
	while(pojazdy.next()) {
		QString nazwaPojazdu = pojazdy.value(1).toString();

		this->plansza->dodajSpecyfikacje(SpecyfikacjaPojazdu(
			QPixmap("dane/pojazdy/" + nazwaPojazdu + "Korpus.png"),
			QPixmap("dane/pojazdy/" + nazwaPojazdu + "Wieza.png"),
			pojazdy.value(2).toFloat(),
			pojazdy.value(3).toFloat(),
			pojazdy.value(4).toFloat(),
			pojazdy.value(5).toFloat(),
			pojazdy.value(6).toInt()
		));
	}

	QSqlQuery pociski("SELECT * FROM pociski");
	while(pociski.next()) {
		this->plansza->dodajSpecyfikacje(SpecyfikacjaPocisku(
			QPixmap("dane/pociski/" + pociski.value(1).toString() + ".png"),
			pociski.value(3).toInt(),
			pociski.value(4).toInt(),
			pociski.value(5).toInt(),
			pociski.value(6).toInt()
		));
	}

	QString nazwaAnimacji = "animacjaTestowa";
	this->plansza->dodajSpecyfikacje(
		SpecyfikacjaAnimacji(
			QPixmap(nazwaAnimacji + ".png"),	//	tekstury
			QSize(4, 4),	//	iloscKlatek
			1000	//	czasTrwaniaMilisekundy
		)
	);
}

void Silnik::odswiezMenu(int milisekundy){
	Menu::Akcja akcja = Menu::BRAK;

	if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::GORA)
		akcja = Menu::GORA;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::DOL)
		akcja = Menu::DOL;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::PRAWO)
		akcja = Menu::PRAWO;
	else if(urzadzenieWejscia->statusNawigatorWcisniecie() & UrzadzenieWejscia::LEWO)
		akcja = Menu::LEWO;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::USUN))
		akcja = Menu::USUN;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYBIERZ))
		akcja = Menu::WYBIERZ;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYBIERZ_ALT))
		akcja = Menu::WYBIERZ_ALT;
	else if(urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::COFNIJ))
		akcja = Menu::COFNIJ;

	this->tryb = this->menu->odswiez(milisekundy, akcja);
	this->menu->rysuj();
}

void Silnik::odswiezRozgrywke(int milisekundy){
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::PAUZA)) {
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

	int zmianaZasiegu = 0;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::GORA)
		zmianaZasiegu++;
	if(this->urzadzenieWejscia->statusNawigatorPolozenie() & UrzadzenieWejscia::DOL)
		zmianaZasiegu--;

	int zmianaBroni = 0;
	if(
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_PLUS) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::BRON_PLUS2)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_PLUS2) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::BRON_PLUS))
	)
		zmianaBroni++;
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::BRON_MINUS))
		zmianaBroni--;

	bool wystrzal = (
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYSTRZAL) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WYSTRZAL2)) ||
		(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYSTRZAL2) && !this->urzadzenieWejscia->statusPrzyciskPolozenie(UrzadzenieWejscia::WYSTRZAL))
	);

	this->logika->odswiez(milisekundy, predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaZasiegu, zmianaBroni, wystrzal);

	//	sprawdzic czy koniec gry

	this->plansza->rysuj();
}

void Silnik::odswiezPauze(int milisekundy)
{
	if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::PAUZA)) {
		this->tryb = ROZGRYWKA;
		return;
	} else if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::COFNIJ)) {
		this->tryb = MENU;
		return;
	} else if(this->urzadzenieWejscia->statusPrzyciskWcisniecie(UrzadzenieWejscia::WYBIERZ)) {
		this->tryb = WYJSCIE;
		return;
	}

	this->plansza->rysuj();

	QPainter painter(&this->ekran->buforObrazu);
	painter.drawText(this->ekran->buforObrazu.rect(), "!!PAUZA!!", QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));
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

	int fps = 1000 / milisekundy;
	QPainter painter(&this->ekran->buforObrazu);
	painter.setFont(QFont("Trebuchet MS", 24));
	painter.drawText(QRectF(0, 0, this->ekran->buforObrazu.width(), 100), QString("FPS: %1").arg(fps), QTextOption(Qt::AlignRight));

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
