#ifndef SILNIK_H
#define SILNIK_H

#include <QTime>

class UrzadzenieWejscia;
class BazaDanych;
class Ladowanie;
class Plansza;
class Logika;
class Ekran;
class Pauza;
class Menu;

class Silnik: public QObject{
	Q_OBJECT

	public:
		enum Tryb{
			LADOWANIE = 0,
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
		Ladowanie *ladowanie;
		Pauza* pauza;
		Logika* logika;
		QTime czasOstatniegoOdswiezenia;
		int ramki;
		int milisekundy;
		int fps;

		void zaladujSpecyfikacjeObiektow();
		void odswiezMenu(int);
		void odswiezRozgrywke(int);
		void odswiezPauze(int);
		void odswiezLadowanie(float, const QString &opis);
		Akcja aktualnaAkcja() const;

	public slots:
		void odswiez();

	public:
		Silnik();
		~Silnik();
		void uruchom();
		bool czyWyjsc() const;
		Tryb tryb;
};

#endif // SILNIK_H
