#include "logika.h"
#include "plansza.h"
#include <QDebug>	//	DEBUG

Logika::Logika(Plansza* plansza):
	plansza(plansza){
}

void Logika::odswiez(double predkoscGasienicyLewej, double predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaZasiegu, int zmianaBroni, bool wystrzal){
	qDebug()
	<< "L: " << predkoscGasienicyLewej
	<< "R: " << predkoscGasienicyPrawej
	<< "W: " << rotacjaWiezy
	<< "D: " << zmianaZasiegu
	<< "B: " << zmianaBroni
	<< "F: " << wystrzal;
}
