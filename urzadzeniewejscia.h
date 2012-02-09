#ifndef URZADZENIEWEJSCIA_H
#define URZADZENIEWEJSCIA_H

class UrzadzenieWejscia{
	public:
		enum Nawigator{
			GORA = 0x01,
			PRAWO = 0x02,
			DOL = 0x04,
			LEWO = 0x08
		};

		enum Dzojstik{
		  LEWY_PIONOWY = 1,
		  LEWY_POZIOMY,
		  PRAWY_PIONOWY,
		  PRAWY_POZIOMY
		};

		enum Przycisk{
			NIEZNANY = -1,

			//gra
			BRON_PLUS = 0,
			BRON_PLUS2,
			BRON_MINUS,
			WIEZA_LEWO,
			WIEZA_PRAWO,
			WYSTRZAL,
			WYSTRZAL2,
			PAUZA,

			//menu
			COFNIJ,
			USUN,
			WYBIERZ,
			WYBIERZ_ALT
		};

		virtual bool otworz() = 0;
		virtual void odswiez() = 0;
		virtual bool statusPrzyciskPolozenie(Przycisk) const = 0;
		virtual bool statusPrzyciskWcisniecie(Przycisk) const = 0;
		virtual int statusNawigatorPolozenie() const = 0;
		virtual int statusNawigatorWcisniecie() const = 0;
		virtual float statusDzojstik(Dzojstik) const = 0;

		class Silnik *silnik;
};

#endif // URZADZENIEWEJSCIA_H
