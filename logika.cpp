#include <cmath>
#include <QVector2D>
#include "logika.h"
#include "plansza.h"

Logika::Logika(Plansza* plansza):
	plansza(plansza){
}

void Logika::odswiez(int milisekundy, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaZasiegu, int zmianaBroni, bool wystrzal){
	float czas = float(milisekundy) / 1000.0;
	
	this->przemiesc(*this->plansza->pojazdGracza, predkoscGasienicyLewej, predkoscGasienicyPrawej, czas);
	
	if(rotacjaWiezy != 0)
		this->plansza->pojazdGracza->zwrotWiezy = fmod(this->plansza->pojazdGracza->zwrotWiezy + rotacjaWiezy * this->plansza->pojazdGracza->specyfikacja->predkoscRotacjiWiezy * czas, 2.0 * M_PI);
	
/*	qDebug()
	<< "L: " << predkoscGasienicyLewej
	<< "R: " << predkoscGasienicyPrawej
	<< "W: " << rotacjaWiezy
	<< "D: " << zmianaZasiegu
	<< "B: " << zmianaBroni
	<< "F: " << wystrzal;*/
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
		nowaPozycja = pojazd.pozycja + QPointF(cos(pojazd.zwrotKorpusu), -sin(pojazd.zwrotKorpusu)) * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyLewej * czas;
		nowyZwrot = pojazd.zwrotKorpusu;
	}else{
		float drogaGasienicyLewej = pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyLewej * czas;
		float drogaGasienicyPrawej = pojazd.specyfikacja->predkoscMaksymalnaPojazdu * predkoscGasienicyPrawej * czas;
		float drogaPojazdu = (drogaGasienicyLewej + drogaGasienicyPrawej) / 2.0;
		float promienSkretu = pojazd.specyfikacja->rozmiar.width() * drogaPojazdu / (drogaGasienicyPrawej - drogaGasienicyLewej);
		float katSkretu = drogaPojazdu / promienSkretu;
		
		QVector2D wektorZwrotZnormalizowany(cos(pojazd.zwrotKorpusu), sin(pojazd.zwrotKorpusu));
		QVector2D wektorPrzesuniecieWzgledne(promienSkretu * (1.0 - cos(katSkretu)), promienSkretu * sin(katSkretu));
		QVector2D wektorPrzesuniecieBezwzgledne(
			wektorZwrotZnormalizowany.x() * wektorPrzesuniecieWzgledne.y() + wektorZwrotZnormalizowany.y() * wektorPrzesuniecieWzgledne.x(),
			wektorZwrotZnormalizowany.y() * wektorPrzesuniecieWzgledne.y() - wektorZwrotZnormalizowany.x() * wektorPrzesuniecieWzgledne.x()
		);
		
		nowaPozycja = pojazd.pozycja + QPointF(wektorPrzesuniecieBezwzgledne.x(), -wektorPrzesuniecieBezwzgledne.y());
		nowyZwrot = fmod(pojazd.zwrotKorpusu + katSkretu, 2.0 * M_PI);
	}
	
	if(this->sprawdzKolizje(pojazd.specyfikacja->rozmiar, nowaPozycja, nowyZwrot))
		return false;
		
	pojazd.pozycja = nowaPozycja;
	pojazd.zwrotKorpusu = nowyZwrot;
	return true;
}
