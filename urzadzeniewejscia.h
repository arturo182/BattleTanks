#ifndef URZADZENIEWEJSCIA_H
#define URZADZENIEWEJSCIA_H

#include <SDL/SDL.h>

#undef main
#define DZOJSTIK_EPSILON 1.0E-2

class UrzadzenieWejscia{
	public:
		enum Nawigator{
			GORA = SDL_HAT_UP,
			DOL = SDL_HAT_DOWN,
			PRAWO = SDL_HAT_RIGHT,
			LEWO = SDL_HAT_LEFT
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
};

#endif // URZADZENIEWEJSCIA_H
