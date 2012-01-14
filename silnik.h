#ifndef SILNIK_H
#define SILNIK_H

#include <QTime>

class UrzadzenieWejscia;
class BazaDanych;
class Plansza;
class Logika;
class Ekran;
class Pauza;
class Menu;

class Silnik: public QObject{
	Q_OBJECT

	public:
		enum Tryb{
			MENU,
			ROZGRYWKA,
			PAUZA,
			WYJSCIE
		};
		enum Akcja{
			GORA = 0,
			DOL,
			PRAWO,
			LEWO,
			WYBIERZ,
			WYBIERZ_ALT,
			USUN,
			COFNIJ,
			START,
			BRAK
		};

	private:
		UrzadzenieWejscia* urzadzenieWejscia;
		Ekran* ekran;
		BazaDanych* bazaDanych;
		Plansza* plansza;
		Menu* menu;
		Pauza* pauza;
		Logika* logika;
		Tryb tryb;
		QTime czasOstatniegoOdswiezenia;
		int ramki;
		int milisekundy;
		int fps;

		void zaladujSpecyfikacjeObiektow();
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		void odswiezPauze(int);
		Akcja aktualnaAkcja() const;

	public slots:
		void odswiez();

	public:
		Silnik();
		~Silnik();
		void uruchom();
		bool czyWyjsc() const;
};

#endif // SILNIK_H
