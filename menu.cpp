#include "menu.h"

Menu::Menu(Ekran& ekran, BazaDanych& bazaDanych, Plansza& plansza):
	ekran(ekran),
	bazaDanych(bazaDanych),
	plansza(plansza){
	this->zaladujSpecyfikecjeObiektow();
}

void Menu::zaladujSpecyfikecjeObiektow(){
	//	dodac specyfikacje obiektow do planszy
	
	
	//	DEMO
	
	QString nazwaPojazdu = "pojazdTestowy";
	this->plansza.dodajSpecyfikacje(
		SpecyfikacjaPojazdu(
			QPixmap(nazwaPojazdu + "Korpus.png"),	//	teksturaKorpus
			QPixmap(nazwaPojazdu + "Wieza.png"),	//	teksturaWieza
			50,		//	przesuniecieWiezy
			100,	//	predkoscMaksymalna
			100		//	wytrzymalosc
		)
	);
	
	QString nazwaAnimacji = "animacjaTestowa";
	this->plansza.dodajSpecyfikacje(
		SpecyfikacjaAnimacji(
			QPixmap(nazwaAnimacji + ".png"),	//	tekstury
			QSize(4, 4)	//	iloscKlatek
		)
	);
}
