#include "animacja.h"

void Animacja::rysuj_klatke(QPainter& painter, QPoint przesuniecie, QRect widok, int numer_klatki) const{
	painter.drawPixmap(
		this->pozycja - przesuniecie + widok.topLeft() - QPoint(this->tekstury.rozmiar_klatki.width() / 2, this->tekstury.rozmiar_klatki.height() / 2),
		this->tekstury,
		QRect(QPoint((numer_klatki % this->tekstury.ilosc_klatek.width()) * this->tekstury.rozmiar_klatki.width(), (numer_klatki / this->tekstury.ilosc_klatek.height()) * this->tekstury.rozmiar_klatki.height()), this->tekstury.rozmiar_klatki));
}

Animacja::Animacja(QPoint pozycja, Tekstury& tekstury, QTime czas_rozpoczecia, int czas_trwania, bool powtarzaj):
	tekstury(tekstury),
	czas_rozpoczecia(czas_rozpoczecia),
	czas_trwania(czas_trwania),
	powtarzaj(powtarzaj),
	aktywna(true),
	pozycja(pozycja){}

bool Animacja::rysuj(QPainter& painter, QPoint przesuniecie, QRect widok, QTime czas_aktualny){
	if(!this->aktywna || (!this->powtarzaj && this->czas_rozpoczecia.msecsTo(czas_aktualny) >= this->czas_trwania))
		return false;
	this->rysuj_klatke(painter, przesuniecie, widok, this->tekstury.liczba_klatek() * (this->czas_rozpoczecia.msecsTo(czas_aktualny) % this->czas_trwania) / this->czas_trwania);
	return true;
}
