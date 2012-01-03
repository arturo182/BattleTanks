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
			ROZGRYWKA,
			PAUZA,
			WYJSCIE
		};

	private:
		UrzadzenieWejscia* urzadzenieWejscia;
		Ekran* ekran;
		BazaDanych* bazaDanych;
		Plansza* plansza;
		Menu* menu;
		Logika* logika;
		Tryb tryb;
		QTime czasOstatniegoOdswiezenia;

		void zaladujSpecyfikacjeObiektow();
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		void odswiezPauze(int);

	public slots:
		void odswiez();

	public:
		Silnik();
		~Silnik();
		void uruchom();
		bool czyWyjsc() const;
};

#endif // SILNIK_H
