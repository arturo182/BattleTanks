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
	
	if(this->plansza->status == Plansza::ROZGRYWKA_ZAKONCZONA)
		return;
	if(this->plansza->status == Plansza::NIEZAINICJALIZOWANA)
		this->inicjalizuj();
	
	
	for(int i = this->plansza->animacje.size() - 1; i >= 0; i--){
		this->plansza->animacje[i]->odswiez(milisekundy);
		
		if(!this->plansza->animacje[i]->sprawdzStatus()){
			delete this->plansza->animacje[i];
			this->plansza->animacje.removeAt(i);
		}
	}
	
	float odleglosc;
	int obrazenia;
	
	for(int i = this->plansza->pociski.size() - 1; i >= 0; i--){
		this->plansza->pociski[i]->odswiez(milisekundy);
		
		if(!this->plansza->pociski[i]->sprawdzStatus()){
			odleglosc = this->odleglosc(this->plansza->pojazdGracza->pozycja, this->plansza->pociski[i]->pozycja);
			if(odleglosc < this->plansza->pociski[i]->specyfikacja->promienRazenia){
				obrazenia = this->plansza->pociski[i]->specyfikacja->silaRazenia * odleglosc / this->plansza->pociski[i]->specyfikacja->promienRazenia;
				this->plansza->pojazdGracza->zdrowie -= obrazenia;
			}
			
			for(QList<PojazdObcy*>::iterator j = this->plansza->pojazdyObce.begin(); j < this->plansza->pojazdyObce.end(); j++){
				odleglosc = this->odleglosc((*j)->pozycja, this->plansza->pociski[i]->pozycja);
				if(odleglosc < this->plansza->pociski[i]->specyfikacja->promienRazenia){
					obrazenia = this->plansza->pociski[i]->specyfikacja->silaRazenia * odleglosc / this->plansza->pociski[i]->specyfikacja->promienRazenia;
					(*j)->zdrowie -= obrazenia;
				}
			}
			
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
	
	if(this->plansza->pojazdGracza->zdrowie <= 0){
		this->plansza->status = Plansza::ROZGRYWKA_ZAKONCZONA;
		this->plansza->wygrana = false;
		return;
	}
	
	this->odswiezPojazdGracza(predkoscGasienicyLewej, predkoscGasienicyPrawej, rotacjaWiezy, zmianaBroni, zmianaZasiegu, wystrzal, czas);
	this->odswiezObcePojazdy(milisekundy, czas);
	
	switch(this->plansza->tryb){
		case Plansza::DEMOLKA:
			if(this->plansza->pojazdyObce.empty()){
				this->plansza->status = Plansza::ROZGRYWKA_ZAKONCZONA;
				this->plansza->wygrana = true;
			}
			break;
		case Plansza::LABIRYNT:
			if(this->odleglosc(this->plansza->pojazdGracza->pozycja, this->plansza->wyjscie) < ODLEGLOSC_OD_PUNKTU_WYJSCIA){
				this->plansza->status = Plansza::ROZGRYWKA_ZAKONCZONA;
				this->plansza->wygrana = true;
			}
			break;
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

void Logika::inicjalizuj() const{
	this->plansza->pojazdGracza->otoczka = this->wyznaczOtoczke(*this->plansza->pojazdGracza);
	this->plansza->pojazdGracza->najblizszyWierzcholek = this->plansza->graf.najblizszyWierzcholek(this->plansza->pojazdGracza->pozycja);
	this->plansza->graf.ustawKorzen(this->plansza->pojazdGracza->najblizszyWierzcholek);
	this->plansza->pojazdGracza->ustawBron();
	
	for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i < this->plansza->pojazdyObce.end(); i++)
		(*i)->otoczka = this->wyznaczOtoczke(**i);
		
	this->plansza->status = Plansza::ROZGRYWKA_TRWA;
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
	
	if(rodzajeKolizji & POJAZDY_NA_SCIEZKACH){
		for(QList<PojazdObcy*>::iterator i = this->plansza->pojazdyObce.begin(); i != this->plansza->pojazdyObce.end(); i++)
			if(*i != &pojazd && this->odleglosc(pojazd.pozycja, (*i)->pozycja) < 0.6 * (sqrt(pow(pojazd.specyfikacja->rozmiarKorpus.width(), 2.0) + pow(pojazd.specyfikacja->rozmiarKorpus.height(), 2.0)) + sqrt(pow((*i)->specyfikacja->rozmiarKorpus.width(), 2.0) + pow((*i)->specyfikacja->rozmiarKorpus.height(), 2.0))))
				return true;
		
		if(this->plansza->pojazdGracza && this->sprawdzOtoczki(pojazd.otoczka, this->plansza->pojazdGracza->otoczka))
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
		

	Dzwiek::odtworz(this->plansza->specyfikacjePociskow[wystrzelonyPocisk]->dzwiekWystrzalu);
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

void Logika::wybierzSciezke(PojazdObcy& pojazd){
	if(this->odleglosc(this->plansza->pojazdGracza->pozycja, pojazd.pozycja) > this->plansza->specyfikacjePociskow[pojazd.aktualnaBron]->zasieg){
		if(this->pojazdNaWierzcholku(pojazd)){
			if(pojazd.w != this->plansza->graf.nastepnyWierzcholek(pojazd.v)){
				pojazd.w = this->plansza->graf.nastepnyWierzcholek(pojazd.v);
				pojazd.ustawZwrot = true;
			}
		}else if(this->plansza->graf.odleglosc(pojazd.v) + this->odleglosc(pojazd.pozycja, this->plansza->graf.pozycjaWierzcholka(pojazd.v)) < this->plansza->graf.odleglosc(pojazd.w) + this->odleglosc(pojazd.pozycja, this->plansza->graf.pozycjaWierzcholka(pojazd.w))){
			int tmp = pojazd.w;
			pojazd.w = pojazd.v;
			pojazd.v = tmp;
			pojazd.ustawZwrot = true;
		}
	}else if(pojazd.w < 0 && this->plansza->graf.wierzcholki[pojazd.v]->krawedzie.size() > 0){
		int iloscSciezek = this->plansza->graf.wierzcholki[pojazd.v]->krawedzie.size();
		
		if(iloscSciezek == 1)
			pojazd.w = this->plansza->graf.wierzcholki[pojazd.v]->krawedzie.first();
		else if(pojazd.vPoprzedni < 0)
			pojazd.w = this->plansza->graf.wierzcholki[pojazd.v]->krawedzie[rand() % iloscSciezek];
		else{
			int pozycjaPoprzedniego = this->plansza->graf.wierzcholki[pojazd.v]->krawedzie.indexOf(pojazd.vPoprzedni);
			int pozycjaWylosowana = rand() % (iloscSciezek - 1);
			
			if(pozycjaWylosowana >= pozycjaPoprzedniego)
				pozycjaWylosowana++;
			pojazd.w = this->plansza->graf.wierzcholki[pojazd.v]->krawedzie[pozycjaWylosowana];
		}
		pojazd.ustawZwrot = true;
	}
}

void Logika::ustawZwrotKorpusu(PojazdObcy& pojazd, float czas){
	float poprzedniZwrotKat = pojazd.zwrotKorpusuKat;
	QVector2D poprzedniZwrotWektor = pojazd.zwrotKorpusuWektor;
	QPolygonF poprzedniaOtoczka = pojazd.otoczka;
	QPoint punktDocelowy = this->plansza->graf.pozycjaWierzcholka(pojazd.w);
	float zwrotKatDocelowy, odchylenieKat, rotacjaKat;
	
	zwrotKatDocelowy = atan2(pojazd.pozycja.y() - punktDocelowy.y(), punktDocelowy.x() - pojazd.pozycja.x());
	if(zwrotKatDocelowy < 0.0)
		zwrotKatDocelowy += 2.0 * M_PI;
	
	odchylenieKat = zwrotKatDocelowy - pojazd.zwrotKorpusuKat;
	rotacjaKat = 2.0 * WSPOLCZYNNIK_PREDKOSCI_OBCYCH_POJAZDOW * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas / pojazd.specyfikacja->rozmiarKorpus.width();
	
	if(odchylenieKat > M_PI)
		odchylenieKat -= 2.0 * M_PI;
	else if(odchylenieKat <= -M_PI)
		odchylenieKat += 2.0 * M_PI;
	
	if(rotacjaKat < fabs(odchylenieKat))
		pojazd.zwrotKorpusuKat = fmod(pojazd.zwrotKorpusuKat + (odchylenieKat > 0 ? rotacjaKat : -rotacjaKat), 2.0 * M_PI);
	else{
		pojazd.zwrotKorpusuKat = zwrotKatDocelowy;
		pojazd.ustawZwrot = false;
	}
	
	pojazd.zwrotKorpusuWektor = QVector2D(cos(pojazd.zwrotKorpusuKat), -sin(pojazd.zwrotKorpusuKat));
	pojazd.otoczka = this->wyznaczOtoczke(pojazd);
	
	if(this->sprawdzKolizje(pojazd, POJAZDY)){
		pojazd.zwrotKorpusuKat = poprzedniZwrotKat;
		pojazd.zwrotKorpusuWektor = poprzedniZwrotWektor;
		pojazd.otoczka = poprzedniaOtoczka;
		pojazd.ustawZwrot = true;
	}
}

void Logika::przemiescKorpusNaSciezce(PojazdObcy& pojazd, float czas){
	QPointF poprzedniaPozycja = pojazd.pozycja;
	QPolygonF poprzedniaOtoczka = pojazd.otoczka;
	float odlegloscWierzcholka = this->odleglosc(pojazd.pozycja, this->plansza->graf.pozycjaWierzcholka(pojazd.w));
	float drogaPojazdu = WSPOLCZYNNIK_PREDKOSCI_OBCYCH_POJAZDOW * pojazd.specyfikacja->predkoscMaksymalnaPojazdu * czas;
	bool dotarlDoCelu;
	
	if(drogaPojazdu < odlegloscWierzcholka){
		pojazd.pozycja += drogaPojazdu * pojazd.zwrotKorpusuWektor.toPointF();
		dotarlDoCelu = false;
	}else{
		pojazd.pozycja = this->plansza->graf.pozycjaWierzcholka(pojazd.w);
		dotarlDoCelu = true;
	}
	
	pojazd.otoczka = this->wyznaczOtoczke(pojazd);
	
	if(this->sprawdzKolizje(pojazd, POJAZDY_NA_SCIEZKACH)){
		pojazd.pozycja = poprzedniaPozycja;
		pojazd.otoczka = poprzedniaOtoczka;
		
		if(rand() % 2){
			int tmp = pojazd.w;
			pojazd.w = pojazd.v;
			pojazd.v = tmp;
			pojazd.vPoprzedni = -1;
			pojazd.ustawZwrot = true;
		}else
			pojazd.licznik = CZAS_OCZEKIWANIA_OBCYCH_POJAZDOW;
	}else if(dotarlDoCelu){
		pojazd.vPoprzedni = pojazd.v;
		pojazd.v = pojazd.w;
		pojazd.w = -1;
	}
}

void Logika::odswiezObcePojazdy(int milisekundy, float czas){
	for(int i = this->plansza->pojazdyObce.size() - 1; i >= 0; i--){
		if(this->plansza->pojazdyObce[i]->zdrowie <= 0){
			delete this->plansza->pojazdyObce[i];
			this->plansza->pojazdyObce.removeAt(i);
		}else if(this->odleglosc(this->plansza->pojazdGracza->pozycja, this->plansza->pojazdyObce[i]->pozycja) < PROMIEN_AKTYWNOSCI_OBCYCH_POJAZDOW){
			if(this->plansza->pojazdyObce[i]->licznik == 0){
				this->wybierzSciezke(*this->plansza->pojazdyObce[i]);
				if(this->plansza->pojazdyObce[i]->w >= 0){
					if(this->plansza->pojazdyObce[i]->ustawZwrot)
						this->ustawZwrotKorpusu(*this->plansza->pojazdyObce[i], czas);
					else
						this->przemiescKorpusNaSciezce(*this->plansza->pojazdyObce[i], czas);
				}
				
				//	aktualizacja
				this->plansza->pojazdyObce[i]->zwrotWiezyWektor = QVector2D(
					cos(this->plansza->pojazdyObce[i]->zwrotKorpusuKat + this->plansza->pojazdyObce[i]->zwrotWiezyWzgledemKorpusuKat),
					-sin(this->plansza->pojazdyObce[i]->zwrotKorpusuKat + this->plansza->pojazdyObce[i]->zwrotWiezyWzgledemKorpusuKat)
				);
			}else if(this->plansza->pojazdyObce[i]->licznik > milisekundy)
				this->plansza->pojazdyObce[i]->licznik -= milisekundy;
			else
				this->plansza->pojazdyObce[i]->licznik = 0;
		}else if(this->plansza->pojazdyObce[i]->licznik > 0)
			this->plansza->pojazdyObce[i]->licznik = 0;
	}
}