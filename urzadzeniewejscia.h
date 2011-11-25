#ifndef URZADZENIEWEJSCIA_H
#define URZADZENIEWEJSCIA_H

#include <SDL/SDL.h>

#undef main

enum Nawigator{
	GORA = SDL_HAT_UP,
	DOL = SDL_HAT_DOWN,
	PRAWO = SDL_HAT_RIGHT,
	LEWO = SDL_HAT_LEFT
};

class UrzadzenieWejscia{
	private:
		SDL_Joystick* urzadzenie;
		int iloscPrzyciskow;
		int iloscNawigatorow;
		int iloscDzojstikow;
		bool* przyciskiPolozenie;
		bool* przyciskiWcisniete;
		int* nawigatory;
		short int* dzojstiki;
		
	public:
		UrzadzenieWejscia();
		~UrzadzenieWejscia();
		bool otworz(int);
		void odswiez();
		bool statusPrzyciskPolozenie(int) const;
		bool statusPrzyciskWcisniecie(int) const;
		double statusDzojstik(int) const;
		int statusNawigator(int) const;
};

#endif // URZADZENIEWEJSCIA_H
