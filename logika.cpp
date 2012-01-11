#include <cmath>
#include "logika.h"
#include "plansza.h"
#include "pojazdgracza.h"
#include "pojazdobcy.h"
#include "pocisk.h"
#include <QDebug>

Logika::Logika(Plansza* plansza):
	plansza(plansza){}

void Logika::odswiez(int milisekundy, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaZasiegu, int zmianaBroni, bool wystrzal){
	float czas = float(milisekundy) / 1000.0;
	
	this->przemiescKorpus(*this->plansza->pojazdGracza, predkoscGasienicyLewej, predkoscGasienicyPrawej, czas, KRAWEDZIE_MAPY | PRZESZKODY | POJAZDY);
	this->obrocWieze(*this->plansza->pojazdGracza, rotacjaWiezy, czas);
	
	if(zmianaZasiegu != 0){
		this->plansza->pojazdGracza->celownikOdleglosc += zmianaZasiegu * PREDKOSC_CELOWNIKA * czas;
		if(this->plansza->pojazdGracza->celownikOdleglosc < MINIMALNA_ODLEGLOSC_CELOWNIKA)
			this->plansza->pojazdGracza->celownikOdleglosc = MINIMALNA_ODLEGLOSC_CELOWNIKA;
		else if(this->plansza->pojazdGracza->celownikOdleglosc > this->plansza->specyfikacjePociskow[this->plansza->pojazdGracza->aktualnaBron].zasieg)
			this->plansza->pojazdGracza->celownikOdleglosc = this->plansza->specyfikacjePociskow[this->plansza->pojazdGracza->aktualnaBron].zasieg;
	}
	
	
	for(QList<Animacja*>::iterator i = this->plansza->animacje.begin(); i != this->plansza->animacje.end(); i++)
		(*i)->odswiez(milisekundy);
		
	for(QList<Pocisk*>::iterator i = this->plansza->pociski.begin(); i != this->plansza->pociski.end(); i++)
		(*i)->odswiez(milisekundy);
		
	if(wystrzal){
		this->plansza->pociski.append(
			new Pocisk(
				&this->plansza->specyfikacjePociskow[this->plansza->pojazdGracza->aktualnaBron],
				true,
				this->plansza->pojazdGracza->punktWylotuLufy(),
				this->plansza->pojazdGracza->zwrotWiezyWektor,
				this->plansza->pojazdGracza->celownikOdleglosc
			)
		);
	}
	
	for(QList<Animacja*>::iterator i = this->plansza->animacje.begin(); i != this->plansza->animacje.end(); i++)
		if(!(*i)->sprawdzStatus()){
			delete *i;
			this->plansza->animacje.removeOne(*i);
		}
	
	for(QList<Pocisk*>::iterator i = this->plansza->pociski.begin(); i != this->plansza->pociski.end(); i++)
		if(!(*i)->sprawdzStatus()){
			this->plansza->animacje.append(
				new Animacja(
					&this->plansza->specyfikacjeAnimacji[0],
					(*i)->pozycja
				)
			);
			delete *i;
			this->plansza->pociski.removeOne(*i);
		}
}

QPolygonF Logika::wyznaczOtoczke(QPointF pozycja, QSize rozmiar, QVector2D zwrot) const{
	QVector2D wektorWysokosci(
		zwrot.x() * rozmiar.height() / 2.0,
		zwrot.y() * rozmiar.height() / 2.0
	);
	QVector2D wektorSzerokosci(
		zwrot.y() * rozmiar.width() / 2.0,
		-zwrot.x() * rozmiar.width() / 2.0
	);
	
	QPolygonF otoczka;
	otoczka << pozycja + wektorWysokosci.toPointF() + wektorSzerokosci.toPointF();
	otoczka << pozycja + wektorWysokosci.toPointF() - wektorSzerokosci.toPointF();
	otoczka << pozycja - wektorWysokosci.toPointF() - wektorSzerokosci.toPointF();
	otoczka << pozycja - wektorWysokosci.toPointF() + wektorSzerokosci.toPointF();
	
	return otoczka;
}

bool Logika::sprawdzOtoczki(QPolygonF& otoczka1, QPolygonF& otoczka2) const{
	if(!otoczka1.boundingRect().intersects(otoczka2.boundingRect()))
		return false;
	
	for(int i = 0; i < otoczka1.size(); i++){
		int j = 1;
		while(j < otoczka2.size() && this->wyznacznikMacierzyWspolliniowosci(otoczka2[j - 1], otoczka2[j], otoczka1[i]) > 0.0)
			j++;
		if(j == otoczka2.size() && this->wyznacznikMacierzyWspolliniowosci(otoczka2.last(), otoczka2.first(), otoczka1[i]) > 0.0)
			return true;
	}
	
	for(int i = 0; i < otoczka2.size(); i++){
		int j = 1;
		while(j < otoczka1.size() && this->wyznacznikMacierzyWspolliniowosci(otoczka1[j - 1], otoczka1[j], otoczka2[i]) > 0.0)
			j++;
		if(j == otoczka1.size() && this->wyznacznikMacierzyWspolliniowosci(otoczka1.last(), otoczka1.first(), otoczka2[i]) > 0.0)
			return true;
	}
	
	return false;
}

bool Logika::sprawdzKolizje(Pojazd& pojazd, int rodzajeKolizji) const{
	if(rodzajeKolizji | KRAWEDZIE_MAPY)
		if(!QRectF(this->plansza->mapa->teksturaOryginalna.rect()).contains(pojazd.otoczka.boundingRect()))
			return true;
	
	if(rodzajeKolizji | PRZESZKODY)
		for(QList<QPolygonF>::iterator i = this->plansza->przeszkody.begin(); i != this->plansza->przeszkody.end(); i++)
			if(this->sprawdzOtoczki(pojazd.otoczka, *i))
				return true;
	
	if(rodzajeKolizji | POJAZDY){
		for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i != this->plansza->pojazdyObce.end(); i++)
			if(*i != &pojazd && this->sprawdzOtoczki(pojazd.otoczka, (*i)->otoczka))
				return true;
		
		if(&pojazd != this->plansza->pojazdGracza && this->sprawdzOtoczki(pojazd.otoczka, this->plansza->pojazdGracza->otoczka))
			return true;
	}
	
	return false;
}

bool Logika::przemiescKorpus(Pojazd& pojazd, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, float czas, int rodzajeKolizji){
	if(predkoscGasienicyLewej == 0.0 && predkoscGasienicyPrawej == 0.0)
		return true;
	
	QPointF poprzedniaPozycja = pojazd.pozycja;
	float poprzedniZwrotKat = pojazd.zwrotKorpusuKat;
	QVector2D poprzedniZwrotWektor = pojazd.zwrotKorpusuWektor;
	QPolygonF poprzedniaOtoczka = pojazd.otoczka;
	
	if(predkoscGasienicyLewej == predkoscGasienicyPrawej)
		pojazd.pozycja += pojazd.zwrotKorpusuWektor.toPointF() * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyLewej * czas;
	else if(predkoscGasienicyLewej == -predkoscGasienicyPrawej)
		pojazd.zwrotKorpusuKat = fmod(pojazd.zwrotKorpusuKat + 2.0 * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyPrawej * czas / pojazd.specyfikacja->rozmiarKorpus.width(), 2.0 * M_PI);
	else{
		float drogaGasienicyLewej = pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyLewej * czas;
		float drogaGasienicyPrawej = pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyPrawej * czas;
		float drogaPojazdu = (drogaGasienicyLewej + drogaGasienicyPrawej) / 2.0;
		float promienSkretu = pojazd.specyfikacja->rozmiarKorpus.width() * drogaPojazdu / (drogaGasienicyPrawej - drogaGasienicyLewej);
		float katSkretu = drogaPojazdu / promienSkretu;
		
		QVector2D wektorPrzesuniecieWzgledne(promienSkretu * (1.0 - cos(katSkretu)), promienSkretu * sin(katSkretu));
		QVector2D wektorPrzesuniecieBezwzgledne(
			pojazd.zwrotKorpusuWektor.x() * wektorPrzesuniecieWzgledne.y() + pojazd.zwrotKorpusuWektor.y() * wektorPrzesuniecieWzgledne.x(),
			pojazd.zwrotKorpusuWektor.y() * wektorPrzesuniecieWzgledne.y() - pojazd.zwrotKorpusuWektor.x() * wektorPrzesuniecieWzgledne.x()
		);
		
		pojazd.pozycja += wektorPrzesuniecieBezwzgledne.toPointF();
		pojazd.zwrotKorpusuKat = fmod(pojazd.zwrotKorpusuKat + katSkretu, 2.0 * M_PI);
	}
	
	pojazd.zwrotKorpusuWektor = QVector2D(cos(pojazd.zwrotKorpusuKat), -sin(pojazd.zwrotKorpusuKat));
	pojazd.otoczka = this->wyznaczOtoczke(pojazd.pozycja, pojazd.specyfikacja->rozmiarKorpus, pojazd.zwrotKorpusuWektor);
	
	if(!this->sprawdzKolizje(pojazd, rodzajeKolizji))
		return true;
	
	pojazd.pozycja = poprzedniaPozycja;
	pojazd.zwrotKorpusuKat = poprzedniZwrotKat;
	pojazd.zwrotKorpusuWektor = poprzedniZwrotWektor;
	pojazd.otoczka = poprzedniaOtoczka;
	return false;
}

void Logika::obrocWieze(Pojazd& pojazd, float rotacja, float czas){
	if(rotacja != 0)
		pojazd.zwrotWiezyWzgledemKorpusuKat = fmod(pojazd.zwrotWiezyWzgledemKorpusuKat + rotacja * pojazd.specyfikacja->predkoscRotacjiWiezy * czas, 2.0 * M_PI);
		
	pojazd.zwrotWiezyWektor = QVector2D(cos(pojazd.zwrotKorpusuKat + pojazd.zwrotWiezyWzgledemKorpusuKat), -sin(pojazd.zwrotKorpusuKat + pojazd.zwrotWiezyWzgledemKorpusuKat));
}
