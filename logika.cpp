#include <ctime>
#include "logika.h"
#include "plansza.h"
#include "pojazdgracza.h"
#include "pojazdobcy.h"
#include "pocisk.h"
#include <QDebug>

Logika::Logika(Plansza* plansza):
	plansza(plansza){
	srand(int(time(0)));
}

void Logika::odswiez(int milisekundy, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaBroni, int zmianaZasiegu, bool wystrzal){
	float czas = float(milisekundy) / 1000.0;
	
	if(!this->plansza->zainicjalizowana)
		this->inicjalizuj();
		
	this->odswiezPojazdGracza(predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaBroni, zmianaZasiegu, wystrzal, czas);
	this->odswiezObcePojazdy(czas);
	
	for(QList<Animacja*>::iterator i = this->plansza->animacje.begin(); i != this->plansza->animacje.end(); i++)
		(*i)->odswiez(milisekundy);
		
	for(QList<Pocisk*>::iterator i = this->plansza->pociski.begin(); i != this->plansza->pociski.end(); i++)
		(*i)->odswiez(milisekundy);
		
	for(QList<Animacja*>::iterator i = this->plansza->animacje.begin(); i != this->plansza->animacje.end(); i++)
		if(!(*i)->sprawdzStatus()){
			delete *i;
			this->plansza->animacje.removeOne(*i);
		}
	
	for(int i = this->plansza->pociski.size() - 1; i >= 0; i--)
		if(!this->plansza->pociski[i]->sprawdzStatus()){
			this->plansza->animacje.append(
				new Animacja(
					this->plansza->pociski[i]->specyfikacja->animacja(),
					this->plansza->pociski[i]->pozycja
				)
			);
			delete this->plansza->pociski[i];
			this->plansza->pociski.removeAt(i);
		}
}

QPolygonF Logika::wyznaczOtoczke(const Pojazd& pojazd) const{
	QVector2D wektorWysokosci(
		pojazd.zwrotKorpusuWektor.x() * pojazd.specyfikacja->rozmiarKorpus.height() / 2.0,
		pojazd.zwrotKorpusuWektor.y() * pojazd.specyfikacja->rozmiarKorpus.height() / 2.0
	);
	QVector2D wektorSzerokosci(
		pojazd.zwrotKorpusuWektor.y() * pojazd.specyfikacja->rozmiarKorpus.width() / 2.0,
		-pojazd.zwrotKorpusuWektor.x() * pojazd.specyfikacja->rozmiarKorpus.width() / 2.0
	);
	
	QPolygonF otoczka;
	otoczka << pojazd.pozycja + wektorWysokosci.toPointF() + wektorSzerokosci.toPointF();
	otoczka << pojazd.pozycja + wektorWysokosci.toPointF() - wektorSzerokosci.toPointF();
	otoczka << pojazd.pozycja - wektorWysokosci.toPointF() - wektorSzerokosci.toPointF();
	otoczka << pojazd.pozycja - wektorWysokosci.toPointF() + wektorSzerokosci.toPointF();
	
	return otoczka;
}

void Logika::inicjalizuj() const{
	this->plansza->pojazdGracza->otoczka = this->wyznaczOtoczke(*this->plansza->pojazdGracza);
	this->plansza->pojazdGracza->najblizszyWierzcholek = this->plansza->graf.najblizszyWierzcholek(this->plansza->pojazdGracza->pozycja);
	this->plansza->graf.ustawKorzen(this->plansza->pojazdGracza->najblizszyWierzcholek);
	this->plansza->pojazdGracza->ustawBron();
	
	for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i < this->plansza->pojazdyObce.end(); i++)
		(*i)->otoczka = this->wyznaczOtoczke(**i);
		
	this->plansza->zainicjalizowana = true;
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
	if(rodzajeKolizji & KRAWEDZIE_MAPY)
		if(!QRectF(this->plansza->mapa->teksturaOryginalna.rect()).contains(pojazd.otoczka.boundingRect()))
			return true;
	
	if(rodzajeKolizji & PRZESZKODY)
		for(QList<QPolygonF>::iterator i = this->plansza->przeszkody.begin(); i != this->plansza->przeszkody.end(); i++)
			if(this->sprawdzOtoczki(pojazd.otoczka, *i))
				return true;
	
	if(rodzajeKolizji & POJAZDY){
		for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i != this->plansza->pojazdyObce.end(); i++)
			if(*i != &pojazd && this->sprawdzOtoczki(pojazd.otoczka, (*i)->otoczka))
				return true;
		
		if(&pojazd != this->plansza->pojazdGracza && this->sprawdzOtoczki(pojazd.otoczka, this->plansza->pojazdGracza->otoczka))
			return true;
	}
	
	return false;
}

bool Logika::przemiescKorpus(Pojazd& pojazd, float predkoscGasienicyLewej, float predkoscGasienicyPrawej, float czas){
	if(predkoscGasienicyLewej == 0.0 && predkoscGasienicyPrawej == 0.0)
		return true;
	
	QPointF poprzedniaPozycja = pojazd.pozycja;
	float poprzedniZwrotKat = pojazd.zwrotKorpusuKat;
	QVector2D poprzedniZwrotWektor = pojazd.zwrotKorpusuWektor;
	QPolygonF poprzedniaOtoczka = pojazd.otoczka;
	
	if(predkoscGasienicyLewej == predkoscGasienicyPrawej)
		pojazd.pozycja += pojazd.zwrotKorpusuWektor.toPointF() * predkoscGasienicyLewej * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas;
	else if(predkoscGasienicyLewej == -predkoscGasienicyPrawej)
		pojazd.zwrotKorpusuKat = fmod(pojazd.zwrotKorpusuKat + 2.0 * predkoscGasienicyPrawej * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas / pojazd.specyfikacja->rozmiarKorpus.width(), 2.0 * M_PI);
	else{
		float drogaGasienicyLewej = predkoscGasienicyLewej * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas;
		float drogaGasienicyPrawej = predkoscGasienicyPrawej * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas;
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
	pojazd.otoczka = this->wyznaczOtoczke(pojazd);
	
	if(!this->sprawdzKolizje(pojazd, KRAWEDZIE_MAPY | PRZESZKODY | POJAZDY))
		return true;
	
	pojazd.pozycja = poprzedniaPozycja;
	pojazd.zwrotKorpusuKat = poprzedniZwrotKat;
	pojazd.zwrotKorpusuWektor = poprzedniZwrotWektor;
	pojazd.otoczka = poprzedniaOtoczka;
	return false;
}

void Logika::obrocWieze(Pojazd& pojazd, int kierunek, float czas){
	pojazd.zwrotWiezyWzgledemKorpusuKat = fmod(pojazd.zwrotWiezyWzgledemKorpusuKat + kierunek * pojazd.specyfikacja->predkoscRotacjiWiezy * czas, 2.0 * M_PI);
}

void Logika::zmienZasieg(Pojazd& pojazd, int kierunek, float czas){
	pojazd.celownikOdleglosc += kierunek * PREDKOSC_CELOWNIKA * czas;
	if(pojazd.celownikOdleglosc < MINIMALNA_ODLEGLOSC_CELOWNIKA)
		pojazd.celownikOdleglosc = MINIMALNA_ODLEGLOSC_CELOWNIKA;
	else if(pojazd.celownikOdleglosc > this->plansza->specyfikacjePociskow[pojazd.aktualnaBron]->zasieg)
		pojazd.celownikOdleglosc = this->plansza->specyfikacjePociskow[pojazd.aktualnaBron]->zasieg;
}

bool Logika::wystrzelPocisk(Pojazd& pojazd, bool pociskGracza){
	int wystrzelonyPocisk = pojazd.wystrzelPocisk();
	
	if(wystrzelonyPocisk < 0)
		return false;
		
	this->plansza->pociski.append(
		new Pocisk(
			this->plansza->specyfikacjePociskow[wystrzelonyPocisk],
			pociskGracza,
			pojazd.punktWylotuLufy(),
			pojazd.zwrotWiezyWektor,
			pojazd.celownikOdleglosc
		)
	);
	return true;
}

void Logika::odswiezPojazdGracza(float predkoscGasienicyLewej, float predkoscGasienicyPrawej, int rotacjaWiezy, int zmianaBroni, int zmianaZasiegu, bool wystrzal, float czas){
	if(predkoscGasienicyLewej != 0.0 || predkoscGasienicyPrawej != 0.0){
		this->przemiescKorpus(*this->plansza->pojazdGracza, predkoscGasienicyLewej, predkoscGasienicyPrawej, czas);
		
		int v = this->plansza->graf.najblizszyWierzcholek(this->plansza->pojazdGracza->pozycja);
		if(v != this->plansza->pojazdGracza->najblizszyWierzcholek){
			this->plansza->graf.ustawKorzen(v);
			this->plansza->pojazdGracza->najblizszyWierzcholek = v;
		}
	}
	
	if(rotacjaWiezy != 0)
		this->obrocWieze(*this->plansza->pojazdGracza, rotacjaWiezy, czas);
	this->plansza->pojazdGracza->zwrotWiezyWektor = QVector2D(
		cos(this->plansza->pojazdGracza->zwrotKorpusuKat + this->plansza->pojazdGracza->zwrotWiezyWzgledemKorpusuKat),
		-sin(this->plansza->pojazdGracza->zwrotKorpusuKat + this->plansza->pojazdGracza->zwrotWiezyWzgledemKorpusuKat)
	);
	
	if(zmianaBroni != 0)
		this->plansza->pojazdGracza->zmienBron(zmianaBroni);
	this->zmienZasieg(*this->plansza->pojazdGracza, zmianaZasiegu, czas);
	
	if(wystrzal)
		this->wystrzelPocisk(*this->plansza->pojazdGracza, true);
}

void Logika::odswiezObcePojazdy(float czas){
	float dystans;

	for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i < this->plansza->pojazdyObce.end(); i++){
		dystans = this->odleglosc(this->plansza->pojazdGracza->pozycja, (*i)->pozycja);
		if(dystans < PROMIEN_AKTYWNOSCI_OBCYCH_POJAZDOW){
		
			if(dystans > this->plansza->specyfikacjePociskow[(*i)->aktualnaBron]->zasieg){
				if(this->pojazdNaWierzcholku(**i)){
					if((*i)->w != this->plansza->graf.nastepnyWierzcholek((*i)->v)){
						(*i)->w = this->plansza->graf.nastepnyWierzcholek((*i)->v);
						(*i)->ustawZwrot = true;
					}
				}else if(this->plansza->graf.odleglosc((*i)->v) + this->odleglosc((*i)->pozycja, this->plansza->graf.pozycjaWierzcholka((*i)->v)) < this->plansza->graf.odleglosc((*i)->w) + this->odleglosc((*i)->pozycja, this->plansza->graf.pozycjaWierzcholka((*i)->w))){
					int tmp = (*i)->w;
					(*i)->w = (*i)->v;
					(*i)->v = tmp;
					(*i)->ustawZwrot = true;
				}
			}else if((*i)->w < 0 && this->plansza->graf.wierzcholki[(*i)->v]->krawedzie.size() > 0){
				(*i)->w = this->plansza->graf.wierzcholki[(*i)->v]->krawedzie[rand() % this->plansza->graf.wierzcholki[(*i)->v]->krawedzie.size()];
				(*i)->ustawZwrot = true;
			}
			
			if((*i)->w >= 0){
				QPointF poprzedniaPozycja = (*i)->pozycja;
				float poprzedniZwrotKat = (*i)->zwrotKorpusuKat;
				QVector2D poprzedniZwrotWektor = (*i)->zwrotKorpusuWektor;
				QPolygonF poprzedniaOtoczka = (*i)->otoczka;
	
				if((*i)->ustawZwrot){
					QPoint punktDocelowy = this->plansza->graf.pozycjaWierzcholka((*i)->w);
					float zwrotKatDocelowy, odchylenieKat, rotacjaKat;
					
					zwrotKatDocelowy = atan2((*i)->pozycja.y() - punktDocelowy.y(), punktDocelowy.x() - (*i)->pozycja.x());
					if(zwrotKatDocelowy < 0.0)
						zwrotKatDocelowy += 2.0 * M_PI;
						
					odchylenieKat = zwrotKatDocelowy - (*i)->zwrotKorpusuKat;
					rotacjaKat = 2.0 * WSPOLCZYNNIK_PREDKOSCI_OBCYCH_POJAZDOW * (*i)->specyfikacja->predkoscMaksymalnaPojazdu * czas / (*i)->specyfikacja->rozmiarKorpus.width();
					
					if(odchylenieKat > M_PI)
						odchylenieKat = 2.0 * M_PI - odchylenieKat;
					else if(odchylenieKat <= -M_PI)
						odchylenieKat += 2.0 * M_PI;
						
					if(rotacjaKat < fabs(odchylenieKat)){
						(*i)->zwrotKorpusuKat = fmod((*i)->zwrotKorpusuKat + (odchylenieKat > 0 ? rotacjaKat : -rotacjaKat), 2.0 * M_PI);
						(*i)->zwrotKorpusuWektor = QVector2D(cos((*i)->zwrotKorpusuKat), -sin((*i)->zwrotKorpusuKat));
						(*i)->otoczka = this->wyznaczOtoczke(**i);
						if(this->sprawdzKolizje(**i, POJAZDY)){
							(*i)->zwrotKorpusuKat = poprzedniZwrotKat;
							(*i)->zwrotKorpusuWektor = poprzedniZwrotWektor;
							(*i)->otoczka = poprzedniaOtoczka;
						}
					}else{
						(*i)->zwrotKorpusuKat = zwrotKatDocelowy;
						(*i)->zwrotKorpusuWektor = QVector2D(cos((*i)->zwrotKorpusuKat), -sin((*i)->zwrotKorpusuKat));
						(*i)->otoczka = this->wyznaczOtoczke(**i);
						if(this->sprawdzKolizje(**i, POJAZDY)){
							(*i)->zwrotKorpusuKat = poprzedniZwrotKat;
							(*i)->zwrotKorpusuWektor = poprzedniZwrotWektor;
							(*i)->otoczka = poprzedniaOtoczka;
						}else
							(*i)->ustawZwrot = false;
					}
				}else{
					float odlegloscWierzcholka = this->odleglosc((*i)->pozycja, this->plansza->graf.pozycjaWierzcholka((*i)->w));
					float drogaPojazdu = WSPOLCZYNNIK_PREDKOSCI_OBCYCH_POJAZDOW * (*i)->specyfikacja->predkoscMaksymalnaPojazdu * czas;
					
					if(drogaPojazdu < odlegloscWierzcholka){
						(*i)->pozycja += drogaPojazdu * (*i)->zwrotKorpusuWektor.toPointF();
						(*i)->otoczka = this->wyznaczOtoczke(**i);
						if(this->sprawdzKolizje(**i, POJAZDY)){
							(*i)->pozycja = poprzedniaPozycja;
							(*i)->otoczka = poprzedniaOtoczka;
						}
					}else{
						(*i)->pozycja = this->plansza->graf.pozycjaWierzcholka((*i)->w);(*i)->otoczka = this->wyznaczOtoczke(**i);
						if(this->sprawdzKolizje(**i, POJAZDY)){
							(*i)->pozycja = poprzedniaPozycja;
							(*i)->otoczka = poprzedniaOtoczka;
						}else{
							(*i)->v = (*i)->w;
							(*i)->w = -1;
						}
					}
				}
				
				//	aktualizacja
				(*i)->zwrotWiezyWektor = QVector2D(
					cos((*i)->zwrotKorpusuKat + (*i)->zwrotWiezyWzgledemKorpusuKat),
					-sin((*i)->zwrotKorpusuKat + (*i)->zwrotWiezyWzgledemKorpusuKat)
				);
			}
		}
	}
}
