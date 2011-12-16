#ifndef SILNIK_H
#define SILNIK_H

#include <QTime>
#include <QTimer>

class UrzadzenieWejscia;
class Ekran;
class BazaDanych;
class Plansza;
class Menu;
class Logika;

class Silnik: public QObject{
	Q_OBJECT
	
	public:
		enum Tryb{
			MENU,
			ROZGRYWKA
		};
		
	private:
		UrzadzenieWejscia* urzadzenieWejscia;
		Ekran* ekran;
		BazaDanych* bazaDanych;
		Plansza* plansza;
		Menu* menu;
		Logika* logika;
		Tryb tryb;
		QTimer timer;
		QTime czasOstatniegoOdswiezenia;
		
		void zaladujSpecyfikecjeObiektow();
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		
	private slots:
		void odswiez();
		
	public:
		Silnik();
		~Silnik();
		void uruchom();
};

#endif // SILNIK_H
