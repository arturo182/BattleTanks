#include <cmath>
#include <QVector2D>
#include "logika.h"
#include "plansza.h"
#include "pojazdgracza.h"
#include "pocisk.h"

Logika::Logika(Plansza* plansza):
	plansza(plansza){
}

void Logika::odswiez(int milisekundy, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaZasiegu, int zmianaBroni, bool wystrzal){
	float czas = float(milisekundy) / 1000.0;
	
	this->przemiesc(*this->plansza->pojazdGracza, predkoscGasienicyLewej, predkoscGasienicyPrawej, czas);
	
	if(rotacjaWiezy != 0)
		this->plansza->pojazdGracza->zwrotWiezyWzgledemKorpusuKat = fmod(this->plansza->pojazdGracza->zwrotWiezyWzgledemKorpusuKat + rotacjaWiezy * this->plansza->pojazdGracza->specyfikacja->predkoscRotacjiWiezy * czas, 2.0 * M_PI);
	
	this->plansza->pojazdGracza->odswiezWektory();
	
	for(QList<Animacja*>::iterator i = this->plansza->animacje.begin(); i != this->plansza->animacje.end(); i++)
		(*i)->odswiez(milisekundy);
		
	for(QList<Pocisk*>::iterator i = this->plansza->pociski.begin(); i != this->plansza->pociski.end(); i++)
		(*i)->odswiez(milisekundy);
		
	if(wystrzal){
		QVector2D wektorPrzesuniecieLufy = this->plansza->pojazdGracza->zwrotWiezyWektor * (this->plansza->pojazdGracza->specyfikacja->przesuniecieOsiDlaWiezy + (this->plansza->pojazdGracza->specyfikacja->rozmiarWieza.height() + this->plansza->specyfikacjePociskow[this->plansza->pojazdGracza->aktualnaBron].rozmiar.height()) / 2.0) - this->plansza->pojazdGracza->zwrotKorpusuWektor * this->plansza->pojazdGracza->specyfikacja->przesuniecieOsiDlaKorpusu;
		this->plansza->pociski.append(
			new Pocisk(
				&this->plansza->specyfikacjePociskow[this->plansza->pojazdGracza->aktualnaBron],
				true,
				this->plansza->pojazdGracza->pozycja + wektorPrzesuniecieLufy.toPointF(),
				this->plansza->pojazdGracza->zwrotWiezyWektor,
				500
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

bool Logika::sprawdzKolizje(QSize rozmiar, QPointF pozycja, float zwrot) const{
	return false;
}

bool Logika::przemiesc(Pojazd& pojazd, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, float czas){
	if(predkoscGasienicyLewej == 0.0 && predkoscGasienicyPrawej == 0.0)
		return true;
	
	QPointF nowaPozycja;
	float nowyZwrot;
	
	if(predkoscGasienicyLewej == predkoscGasienicyPrawej){
		nowaPozycja = pojazd.pozycja + pojazd.zwrotKorpusuWektor.toPointF() * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyLewej * czas;
		nowyZwrot = pojazd.zwrotKorpusuKat;
	}else{
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
		
		nowaPozycja = pojazd.pozycja + wektorPrzesuniecieBezwzgledne.toPointF();
		nowyZwrot = fmod(pojazd.zwrotKorpusuKat + katSkretu, 2.0 * M_PI);
	}
	
	if(this->sprawdzKolizje(pojazd.specyfikacja->rozmiarKorpus, nowaPozycja, nowyZwrot))
		return false;
		
	pojazd.pozycja = nowaPozycja;
	pojazd.zwrotKorpusuKat = nowyZwrot;
	return true;
}
